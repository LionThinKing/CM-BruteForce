#include <windows.h>
#include <stdio.h>
enum
{
	ID_az,
	ID_AZ,
	ID_09,
	ID_CS,
	ID_CUSTOM,
	ID_TCHARSET,
	ID_SCHARSET,
	ID_CHARSET,
	ID_PASLEN,
	ID_SMINLEN,
	ID_SMAXLEN,
	ID_MINLEN,
	ID_MAXLEN,
	ID_VICOPT,
	ID_SURL,
	ID_URL,
	ID_SPARAMETERS,
	ID_PARAMETERS,
	ID_SVALIDATE,
	ID_VALIDATE,
	ID_BUTTON,
	ID_SSTATUS,
	ID_STATUS,
};


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CM BruteForce";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

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
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND+2;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "CM BruteForce 0.1",       /* Title Text */
           WS_EX_TRANSPARENT | WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           525,                 /* The programs width */
           550,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

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


char *GetStringCharSet(HWND hwnd)
{
    char *Temp,*CharSet;
    int Size;
    
    CharSet=(char*)malloc(10000);
    memset(CharSet,0,10000);
    
    
    if(IsDlgButtonChecked(hwnd, ID_CS)== BST_CHECKED)
    {
        Size=GetWindowTextLength(GetDlgItem(hwnd,(ID_CUSTOM)));
        Temp=(char*)malloc(Size+10);
        memset(CharSet,0,Size+9);
        GetWindowText(GetDlgItem(hwnd,(ID_CUSTOM)),Temp,Size+1);
        strcat(CharSet,Temp);
    }
    
    if(IsDlgButtonChecked(hwnd, ID_az)== BST_CHECKED)
    {
        Temp=(char*)malloc(30);
        memset(Temp,0,30);
        strcat(Temp,"abcdefghijklmnñopqrstuvwxyz");
        strcat(CharSet,Temp);
    }
    
    if(IsDlgButtonChecked(hwnd, ID_AZ)== BST_CHECKED)
    {
        Temp=(char*)malloc(30);
        memset(Temp,0,30);
        strcat(Temp,"ABCDEFGHIJKLMNÑOPQRSTUVWXYZ");
        strcat(CharSet,Temp);
    }
    
    if(IsDlgButtonChecked(hwnd, ID_09)== BST_CHECKED)
    {
        Temp=(char*)malloc(30);
        memset(Temp,0,30);
        strcat(Temp,"0123456789");
        strcat(CharSet,Temp);
    }
    
    return CharSet;
}


int GetMinLength(HWND hwnd)
{
    char *Temp=(char*)malloc(3);
    memset(Temp,0,3);
    GetWindowText(GetDlgItem(hwnd,(ID_MINLEN)),Temp,3);
    return atoi(Temp);
}

int GetMaxLength(HWND hwnd)
{
    char *Temp=(char*)malloc(3);
    memset(Temp,0,3);
    GetWindowText(GetDlgItem(hwnd,(ID_MAXLEN)),Temp,3);
    return atoi(Temp);
}


int BruteForce(int Min, int Max, char *Base, char *CharSet)
{
    FILE *File;
    char *Pass;
    char Aux[]="a";
    for(int c=0;c<strlen(CharSet);c++)
    {
        Pass=(char*)malloc(strlen(CharSet)+1);
        memset(Pass,0,strlen(CharSet));
        Aux[0]=CharSet[c];
        strcat(Pass,Base);
        strcat(Pass,Aux);

        if(Min < Max)
        {
            BruteForce(Min+1,Max, Pass,CharSet);
        }
        if(strlen(Pass)<Max+1){continue;}

        File=fopen("brute.txt","a");
        fprintf(File,"%s\n",Pass);
        fclose(File);
    }
}

int Start(HWND hwnd, int Min, int Max)
{
    Min-=1;
    Max-=1;
    EnableWindow(GetDlgItem(hwnd,ID_BUTTON),false);  
    while(Min<=Max)
    {
        BruteForce(0,Min,"",GetStringCharSet(hwnd));
        Min++;    
    }
    MessageBox(0,"Terminado","",0);
    EnableWindow(GetDlgItem(hwnd,ID_BUTTON),true);  
    
}


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
			//Charset
            CreateWindowEx(0,"STATIC", "Add CharSet:", WS_VISIBLE | WS_CHILD, 10, 10, 130, 20,hwnd, (HMENU)(ID_TCHARSET), NULL, NULL);
        
			CreateWindowEx(0,"BUTTON", "Lowercase (a...z)",BS_AUTOCHECKBOX | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, 30, 40, 140, 20,hwnd, (HMENU)ID_az, NULL, NULL);
			CreateWindowEx(0,"BUTTON", "Uppercase (A...Z)",BS_AUTOCHECKBOX | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, 200, 40, 140, 20,hwnd, (HMENU)ID_AZ, NULL, NULL);
			CreateWindowEx(0,"BUTTON", "Digits (0...9)",BS_AUTOCHECKBOX | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, 370, 40, 130, 20,hwnd, (HMENU)ID_09, NULL, NULL);
			CreateWindowEx(0,"BUTTON", "Custom...",BS_AUTOCHECKBOX | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, 30, 80, 100, 25,hwnd, (HMENU)ID_CS, NULL, NULL);
			CreateWindow("EDIT","",ES_AUTOHSCROLL | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,150,80,350,20,hwnd,(HMENU)(ID_CUSTOM),NULL,NULL);
			ShowWindow(GetDlgItem(hwnd,(ID_CUSTOM)),true);
			
			CreateWindowEx(0,"STATIC", "CharSet String:", WS_VISIBLE | WS_CHILD, 30, 120, 100, 20,hwnd, (HMENU)(ID_SCHARSET), NULL, NULL);
			CreateWindow("EDIT","",ES_AUTOHSCROLL | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,150,120,350,20,hwnd,(HMENU)(ID_CHARSET),NULL,NULL);
			EnableWindow(GetDlgItem(hwnd,ID_CHARSET),false);
			
			//Position and Length
			CreateWindowEx(0,"STATIC", "Password Position and Length:", WS_VISIBLE | WS_CHILD, 10, 180, 210, 20,hwnd, (HMENU)(ID_PASLEN), NULL, NULL);
			
			CreateWindowEx(0,"STATIC", "Minimal Length:", WS_VISIBLE | WS_CHILD, 30, 210, 110, 20,hwnd, (HMENU)(ID_SMINLEN), NULL, NULL);
			CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,160,210,20,20,hwnd,(HMENU)(ID_MINLEN),NULL,NULL);
			CreateWindowEx(0,"STATIC", "Maximal Length:", WS_VISIBLE | WS_CHILD, 30, 240, 110, 20,hwnd, (HMENU)(ID_SMAXLEN), NULL, NULL);
			CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,160,240,20,20,hwnd,(HMENU)(ID_MAXLEN),NULL,NULL);
			
			//Victims Options
			CreateWindowEx(0,"STATIC", "Victims Options:", WS_VISIBLE | WS_CHILD, 10, 290, 110, 15,hwnd, (HMENU)(ID_VICOPT), NULL, NULL);
			//URL
			CreateWindowEx(0,"STATIC", "URL:", WS_VISIBLE | WS_CHILD, 30, 320, 40, 20,hwnd, (HMENU)(ID_SURL), NULL, NULL);
			CreateWindow("EDIT","", ES_AUTOHSCROLL | WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,150,320,350,20,hwnd,(HMENU)(ID_URL),NULL,NULL);
			//Parameters
			CreateWindowEx(0,"STATIC", "Parameters:", WS_VISIBLE | WS_CHILD, 30, 360, 80, 20,hwnd, (HMENU)(ID_SPARAMETERS), NULL, NULL);
			CreateWindow("EDIT","",ES_AUTOHSCROLL |WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LTRREADING ,150,360,350,20,hwnd,(HMENU)(ID_PARAMETERS),NULL,NULL);
			//Validate
			CreateWindowEx(0,"STATIC", "Validate Text:", WS_VISIBLE | WS_CHILD, 30, 400, 90, 20,hwnd, (HMENU)(ID_SVALIDATE), NULL, NULL);
			CreateWindow("EDIT","",ES_AUTOHSCROLL |WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LTRREADING ,150,400,350,20,hwnd,(HMENU)(ID_VALIDATE),NULL,NULL);
			
			//START
			CreateWindowEx(0,"BUTTON", "Start!",WS_CHILD | WS_VISIBLE | WS_TABSTOP, 430,440, 70, 25,hwnd, (HMENU)ID_BUTTON, NULL, NULL);
			
			//Status
			CreateWindowEx(0,"STATIC", "Status:", WS_VISIBLE | WS_CHILD, 0, 480, 47, 20,hwnd, (HMENU)(ID_SSTATUS), NULL, NULL);
			CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | WS_EX_LTRREADING ,50,480,450,20,hwnd,(HMENU)(ID_STATUS),NULL,NULL);
			EnableWindow(GetDlgItem(hwnd,ID_STATUS),false);
            
        break;    
        
        case WM_COMMAND:  
                switch(LOWORD(wParam)) 
                {
                    case ID_az:
                    case ID_AZ:
                    case ID_09:
                    case ID_CUSTOM:
                        SetWindowText(GetDlgItem(hwnd,(ID_CHARSET)),GetStringCharSet(hwnd));
                    break;
                        
                    
                                            
                    case ID_CS:
                        if(IsDlgButtonChecked(hwnd, ID_CS)== BST_CHECKED)
                        {
                            ShowWindow(GetDlgItem(hwnd,(ID_CUSTOM)),true);
                        }
                        else
                        {
                            ShowWindow(GetDlgItem(hwnd,(ID_CUSTOM)),false);
                        }
                        break;
                        
                    case ID_BUTTON:
                        Start(hwnd,GetMinLength(hwnd),GetMaxLength(hwnd));
                        //BruteForce(GetMinLength(hwnd), GetMaxLength(hwnd),"",GetStringCharSet(hwnd));
                        break;
                }
                break;
        
            
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

