#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "menu.h"

typedef void(__cdecl *MYPROC1) (HWND, char*, DWORD);
typedef void(__cdecl *MYPROC2) (HWND, char*);

HWND hButtonPlay;
HWND hButtonEndGame;
HWND hTextQuestion;
HWND hButtonAnswer1;
HWND hButtonAnswer2;
HWND hButtonAnswer3;
HWND hButtonAnswer4;
int questionsAnswered;

typedef struct {
    char* text;
    BOOL isCorrect;
} Answer;

typedef struct {
    char* questionText;
    Answer answers[4];
} Question;

Question questions[] = {
    {"Which element is named after the creator of the periodic table?",
     {{"Mendelevium", TRUE}, {"Einsteinium", FALSE}, {"Curium", FALSE}, {"Bohrium", FALSE}}},

    {"What is the capital city of New Zealand?",
     {{"Auckland", FALSE}, {"Wellington", TRUE}, {"Sydney", FALSE}, {"Melbourne", FALSE}}},

    {"Which language is known as the language of love?",
     {{"Italian", FALSE}, {"Spanish", FALSE}, {"French", TRUE}, {"Portuguese", FALSE}}},

    {"What is the only mammal capable of true flight?",
     {{"Flying squirrel", FALSE}, {"Colugo", FALSE}, {"Pterosaur", FALSE}, {"Bat", TRUE}}},

    {"Who composed the Four Seasons concertos?",
     {{"Antonio Vivaldi", TRUE}, {"Johann Sebastian Bach", FALSE}, {"Wolfgang Amadeus Mozart", FALSE}, {"Ludwig van Beethoven", FALSE}}},

    {"In which country was the Caesar salad invented?",
     {{"Italy", FALSE}, {"Mexico", TRUE}, {"Greece", FALSE}, {"Spain", FALSE}}},

    {"What is the hardest natural substance on Earth?",
     {{"Quartz", FALSE}, {"Topaz", FALSE}, {"Diamond", TRUE}, {"Ruby", FALSE}}},

    {"Which bird is known for its impressive mimicry skills?",
     {{"Parrot", FALSE}, {"Mockingbird", FALSE}, {"Raven", FALSE}, {"Lyrebird", TRUE}}},

    {"What is the term for a word that is spelled the same forwards and backwards?",
     {{"Palindrome", TRUE}, {"Anagram", FALSE}, {"Homonym", FALSE}, {"Synonym", FALSE}}},

    {"In Greek mythology, who was the goddess of wisdom?",
     {{"Aphrodite", FALSE}, {"Athena", TRUE}, {"Hera", FALSE}, {"Artemis", FALSE}}},

    {"Which country is the largest producer of coffee in the world?",
     {{"Colombia", FALSE}, {"Vietnam", FALSE}, {"Brazil", TRUE}, {"Ethiopia", FALSE}}},

    {"What is the primary ingredient in traditional hummus?",
     {{"Lentils", FALSE}, {"Beans", FALSE}, {"Peas", FALSE}, {"Chickpeas", TRUE}}},

    {"Which artist is known for the painting 'The Starry Night'?",
     {{"Vincent van Gogh", TRUE}, {"Pablo Picasso", FALSE}, {"Claude Monet", FALSE}, {"Salvador Dali", FALSE}}},

    {"In which sport would you perform a 'Fosbury Flop'?",
     {{"Pole vault", FALSE}, {"High jump", TRUE}, {"Long jump", FALSE}, {"Triple jump", FALSE}}},

    {"Which planet in our solar system has the most moons?",
     {{"Jupiter", FALSE}, {"Uranus", FALSE}, {"Saturn", TRUE}, {"Neptune", FALSE}}},

    {"What is the largest desert on Earth?",
     {{"Sahara", FALSE}, {"Arabian", FALSE}, {"Gobi", FALSE}, {"Antarctica", TRUE}}},

    {"What musical instrument has six strings and is commonly used in rock music?",
     {{"Guitar", TRUE}, {"Violin", FALSE}, {"Piano", FALSE}, {"Drums", FALSE}}},

    {"What is the longest river in the world?",
     {{"Amazon", FALSE}, {"Nile", TRUE}, {"Yangtze", FALSE}, {"Mississippi", FALSE}}},

    {"Which famous scientist developed the theory of general relativity?",
     {{"Isaac Newton", FALSE}, {"Stephen Hawking", FALSE}, {"Albert Einstein", TRUE}, {"Nikola Tesla", FALSE}}},

    {"What is the name of the largest moon of Saturn?",
     {{"Ganymede", FALSE}, {"Callisto", FALSE}, {"Europa", FALSE}, {"Titan", TRUE}}}
};




/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
void DrawImage(HWND);
void HideQuestionButtons(HWND);
void GameOver(HWND, int);
void GameWon(HWND, int);
void LoadQuestions(HWND, int);
void SaveScore(HWND, int);
void LoadScore(HWND, char*, size_t);
void GetCurrentDateTime(char* , int);
void ChooseAnswer(HWND, int);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = /*(HBRUSH) */ CreateSolidBrush(RGB(0,102,255)); //COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Quiz game"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           744,                 /* The programs width */
           575,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(hThisInstance, MAKEINTRESOURCE(IDR_MENU)), //NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hLib;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
                hButtonPlay = CreateWindow("BUTTON", TEXT("Play"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 100, hwnd, (HMENU)BUTTON_PLAY, NULL, NULL);
                CreateWindow("BUTTON", TEXT("History"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 440, 100, 30, hwnd, (HMENU) BUTTON_SCORES, NULL ,NULL);
                CreateWindow("BUTTON", TEXT("Exit"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 480, 100, 30, hwnd, (HMENU)BUTTON_EXIT, NULL, NULL);
                hButtonEndGame = CreateWindow("BUTTON", TEXT("End game"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 10, 100, 100, hwnd, (HMENU) BUTTON_END_GAME, NULL, NULL);
                hTextQuestion = CreateWindow("STATIC", TEXT("Question Text"), WS_VISIBLE | WS_CHILD, 200, 190, 420, 170, hwnd, (HMENU) TEXT_QUESTION, NULL, NULL);
                hButtonAnswer1 = CreateWindow("BUTTON", TEXT("Answer1"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 200, 390, 200, 30, hwnd, (HMENU) BUTTON_ANSWER_1, NULL ,NULL);
                hButtonAnswer2 = CreateWindow("BUTTON", TEXT("Answer2"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 420, 390, 200, 30, hwnd, (HMENU) BUTTON_ANSWER_2, NULL ,NULL);
                hButtonAnswer3 = CreateWindow("BUTTON", TEXT("Answer3"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 200, 430, 200, 30, hwnd, (HMENU) BUTTON_ANSWER_3, NULL ,NULL);
                hButtonAnswer4 = CreateWindow("BUTTON", TEXT("Answer4"), WS_TABSTOP | WS_VISIBLE | WS_CHILD, 420, 430, 200, 30, hwnd, (HMENU) BUTTON_ANSWER_4, NULL ,NULL);
                HideQuestionButtons(hwnd);

                break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case ID_FILE_EXIT:
                    PostQuitMessage(0);
                    break;
                case BUTTON_SCORES:
                    DialogBox(NULL, MAKEINTRESOURCE(ID_DIALOG), hwnd, (DLGPROC) DialogProc);
                    break;
                case BUTTON_PLAY:
                        ShowWindow(hButtonPlay, SW_HIDE);
                        ShowWindow(hButtonEndGame, SW_SHOW);
                        ShowWindow(hTextQuestion, SW_SHOW);
                        ShowWindow(hButtonAnswer1, SW_SHOW);
                        ShowWindow(hButtonAnswer2, SW_SHOW);
                        ShowWindow(hButtonAnswer3, SW_SHOW);
                        ShowWindow(hButtonAnswer4, SW_SHOW);

                        questionsAnswered = 0;
                        LoadQuestions(hwnd, questionsAnswered);
                    break;
                case BUTTON_EXIT:
                    if(MessageBox(hwnd, "Are you sure you want to exit?", "Exit", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONASTERISK) == 1){
                        PostQuitMessage (0);
                    }
                    break;
                case BUTTON_END_GAME:
                    if(MessageBox(hwnd, "Are you sure you want to end the current game?", "End game", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONASTERISK) == 1){
                        HideQuestionButtons(hwnd);
                    }
                    break;
                case BUTTON_ANSWER_1:
                    ChooseAnswer(hwnd, 0);
                    break;
                case BUTTON_ANSWER_2:
                    ChooseAnswer(hwnd, 1);
                    break;
                case BUTTON_ANSWER_3:
                    ChooseAnswer(hwnd, 2);
                    break;
                case BUTTON_ANSWER_4:
                    ChooseAnswer(hwnd, 3);
                    break;
            }

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                DrawImage(hwnd);
                EndPaint(hwnd, &ps);
            }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    char scores[2048];
    char* line;
    switch(msg){
        case WM_INITDIALOG:

            LoadScore(hDlg, scores, sizeof(scores));
            line = strtok(scores, "\n");
            while (line != NULL) {
                SendDlgItemMessage(hDlg, IDC_RESULTS_LIST, LB_ADDSTRING, 0, (LPARAM)line);
                line = strtok(NULL, "\n");
            }
            return true;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case ID_BUTTON:
                    EndDialog(hDlg,0);
                    return true;
            }
            return true;
        case WM_CLOSE:
            EndDialog(hDlg,0);
            return true;
        case WM_DESTROY:
            EndDialog(hDlg,0);
            return true;
    }
    return false;
}

void DrawImage(HWND hwnd){
    HDC hdc;
    hdc = GetDC(hwnd);
    HANDLE bmp = LoadImage(NULL,"quizicon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (bmp==NULL){
        MessageBox(hwnd, "Failed to load image", "Error", MB_OK|MB_ICONERROR);
        return ;
    }
    HDC dcmem = CreateCompatibleDC(NULL);
    if (SelectObject(dcmem, bmp)==NULL){
        MessageBox(hwnd, "Failed to upload", "Error", MB_OK|MB_ICONERROR);
        DeleteDC(dcmem);
        return ;
    }
    BITMAP bm;
    GetObject(bmp, sizeof(bm), &bm);
    if (BitBlt(hdc, 220, 10, bm.bmWidth, bm.bmHeight, dcmem, 0, 0, SRCCOPY) == 0){
        MessageBox(hwnd, "Failed to blit", "Error", MB_OK|MB_ICONERROR);
        DeleteDC(dcmem);
        return ;
    }
    DeleteDC(dcmem);
}

void LoadQuestions(HWND hwnd, int questionsAnswered){
    SetWindowText(hTextQuestion, TEXT(questions[questionsAnswered].questionText));
    SetWindowText(hButtonAnswer1, TEXT(questions[questionsAnswered].answers[0].text));
    SetWindowText(hButtonAnswer2, TEXT(questions[questionsAnswered].answers[1].text));
    SetWindowText(hButtonAnswer3, TEXT(questions[questionsAnswered].answers[2].text));
    SetWindowText(hButtonAnswer4, TEXT(questions[questionsAnswered].answers[3].text));
    return;
}

void GameOver(HWND hwnd, int answered){
    char message[512];
    sprintf(message, "Incorrect!\nGame over!\nQuestions correctly answered: %d",  answered);
    if(MessageBox(hwnd, message, "Game over", MB_OK | MB_DEFBUTTON2 | MB_ICONASTERISK) == 1){
      HideQuestionButtons(hwnd);
    }
    SaveScore(hwnd, answered);
    return;
}

void SaveScore(HWND hwnd, int answered){
    HINSTANCE hLib;
    char message[2048];
    char dateTimeStr[64];
    GetCurrentDateTime(dateTimeStr, sizeof(dateTimeStr));

    hLib = LoadLibrary("ReadWriteResults.dll");
    if(hLib != NULL){
        MYPROC2 WriteDataInNewLine = (MYPROC2) GetProcAddress(hLib, "WriteDataInNewLine");
        if(WriteDataInNewLine != NULL){
            sprintf(message, "%sQuestions answered: %d", dateTimeStr, answered);
            WriteDataInNewLine(hwnd, message);
        } else {
            MessageBox(hwnd, "Something is bad with dll function", "DLL error", MB_OK);
        }
        FreeLibrary(hLib);
    } else {
        MessageBox(hwnd, "Something is bad with dll", "DLL error", MB_OK);
    }
}


void LoadScore(HWND hwnd, char* buffer, size_t bufferSize) {
    HINSTANCE hLib = LoadLibrary("ReadWriteResults.dll");
    if (hLib != NULL) {
        MYPROC1 ReadData = (MYPROC1)GetProcAddress(hLib, "ReadData");
        if (ReadData != NULL) {
            ReadData(hwnd, buffer, bufferSize);
        } else {
            MessageBox(hwnd, "Something is bad with dll function", "DLL error", MB_OK);
        }
        FreeLibrary(hLib);
    } else {
        MessageBox(hwnd, "Something is bad with dll", "DLL error", MB_OK);
    }
}

void GameWon(HWND hwnd, int answered){
    char message[256];
    sprintf(message, "You answered all the questions!\nCongratulations!\nQuestions answered: %d",  answered);
    if(MessageBox(hwnd, message, "Victory", MB_OK | MB_DEFBUTTON2 | MB_ICONASTERISK) == 1){
        HideQuestionButtons(hwnd);
    }
    SaveScore(hwnd, answered);
    return;
}

void HideQuestionButtons(HWND hwnd){
    ShowWindow(hButtonEndGame, SW_HIDE);
    ShowWindow(hButtonPlay, SW_SHOW);
    ShowWindow(hTextQuestion, SW_HIDE);
    ShowWindow(hButtonAnswer1, SW_HIDE);
    ShowWindow(hButtonAnswer2, SW_HIDE);
    ShowWindow(hButtonAnswer3, SW_HIDE);
    ShowWindow(hButtonAnswer4, SW_HIDE);
}

void GetCurrentDateTime(char* dateTimeStr, int maxLen) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(dateTimeStr, maxLen, "%04d-%02d-%02d %02d:%02d:%02d - ",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void ChooseAnswer(HWND hwnd, int answerVariant){
     if (questions[questionsAnswered].answers[answerVariant].isCorrect) {
        questionsAnswered++;
        if (questionsAnswered < sizeof(questions) / sizeof(questions[0])) {
            LoadQuestions(hwnd, questionsAnswered);
        } else {
            GameWon(hwnd, questionsAnswered);
        }
    } else {
    GameOver(hwnd, questionsAnswered);
    }
}
