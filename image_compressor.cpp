#include "comp.h"
#include <ctime>

using namespace std;



int main()
{

    system("CLS"); //Clears the console for better viewing
    cout<<"-----------------------------------"<<endl;
    cout<<"   Welcome to PGM compressor   "<<endl;
    cout<<"                               "<<endl;
    cout<<"#Created by Emir Özen & Erk Demirel"<<endl;
    cout<<"-----------------------------------"<<endl;
    string nameoffile ="";
    int b;
    int offset;
    
        
    do
    {
        cout<<"If you want to compress a file type 1."<<endl;
        cout<<"If you want to decompress a file type 2."<<endl;
        cout<<"If you want compress & decompress type 3."<<endl;
        cin>>b;
        cin.clear(); //We need to clear the input so we can enter a next one
        cin.sync();
        
    } while (b != 1 && b!= 2 && b!=3);

    
    if(b==1)
    {   
        cout<< "Please enter your desired compression degree (default = 20)" <<endl;
        cout<<"(1 = Minimum compression | 200 = Highest compression)"<<endl;
        cin>>offset;
        cin.clear();
        cin.sync();
        cout << "Please enter your filename without extension: " <<endl;
        getline(cin,nameoffile);
        CompressedFile OriginalFile(nameoffile);
        cout<<"\nName of the file = "<<OriginalFile.getName()<<endl;
        cout<<"\nComment in the pgm file = "<<OriginalFile.getComment()<<endl;
        cout<<"\nInformation about the pgm file \n" <<OriginalFile.getHeader()<<endl;
        OriginalFile.Compressor(offset);
        cout << ".ezy file created." <<endl;

    }
    else if(b==2)
    {
        cout << "\nPlease enter your filename without extension: " <<endl;
        getline(cin,nameoffile);
        CompressedFile OriginalFile(nameoffile);
        Decompressor(nameoffile);
        cout << "\nDecompressed file created." << endl;
    }
    else if(b==3)
    {
        cout<< "\nPlease enter your desired compression degree (default = 20)" <<endl;
        cout<<"(1 = Minimum compression | 200 = Highest compression)"<<endl;
        cin>>offset;
        cin.clear();
        cin.sync();
        cout << "Please enter your filename without extension: " <<endl;
        getline(cin,nameoffile);
        CompressedFile OriginalFile(nameoffile);
        cout<<"\nName of the file = "<<OriginalFile.getName()<<endl;
        cout<<"\nComment in the pgm file = "<<OriginalFile.getComment()<<endl;
        cout<<"\nInformation about the pgm file \n"<<OriginalFile.getHeader()<<endl;
        OriginalFile.Compressor(offset);
        cout << "\n.ezy file created." <<endl;
        Decompressor(nameoffile);
        cout << "\nDecompressed file created." << endl;
    }
    cout<<"\nProgram has finished ";

    return 0;
    
}