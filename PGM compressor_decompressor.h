#include <iostream>
#include <string>
#include <fstream>
#include <new>
#include <ctime>
using namespace std;

// -- Decompressing the file we have created with the extension .ezy -- //
// -- This function is out of class because if user wants only decompress the file. -- //
void Decompressor(string name)
{
    clock_t start = clock(); // -- Simple chronometer -- //
    string path = name +".ezy"; // -- It adds the extension -- //
    ifstream reading(path,ios::binary | ios::ate);
    if(reading.is_open())
    {
        streampos fileSize = reading.tellg(); // -- Getting the file size from the .ezy file to create a memory block -- //
        unsigned char* mBlock = new (nothrow) unsigned char[fileSize];
        if(mBlock != nullptr)
        {
            reading.seekg(0,ios::beg); // -- To start reading properly, taking back the handler to the beginning so it doesn't think it finished reading -- //
            reading.read((char*)mBlock,fileSize); // -- Reading from the file as big as file size, writes it into mBlock array --//
            reading.close();

            string header = "P5\n"; // -- Our program works with P5 type so it is default -- // 
            unsigned char* temp = mBlock; // -- If we change the mBlock's adress, we cannot delete it correctly. Because of this we need to copy the first adress into new temp pointer -- //

            // -- In the compressed file, the first bytes contain width and height of the image so we need to get them first -- //
            unsigned short width = *((unsigned short*)temp);
            temp+=sizeof(unsigned short);
            unsigned short height = *((unsigned short*)temp);
            temp+=sizeof(unsigned short);
            fileSize-= (2*sizeof(unsigned short));

            //- Adding the width and height and byte size in grayscale in to the header -- //
            header += to_string(width) + ' ' + to_string(height) + "\n255\n";
            path = name + "decompressed.pgm";

            ofstream file(path,ios::binary);
            if(file.is_open())
            {
                /*for(int b = 0; b<header.length(); b++)
                {
                    file.write((char*)&header[b], sizeof(char));
                }*/
                
                file << header; // -- Putting the header in to the file (both version works) -- //
                


                // -- In this loop we do the decompression part -- //
                // -- We create a loop as big as the image (file size), read how many times the color repeats
                // and then write it in to the decompressed file by chars as big as 1 byte (size of a char) --//
                streampos filesize = width*height;
                while(filesize >0)
                {
                    int sizeofarray = 0;
                    sizeofarray = *temp;
                    temp++;
                    
                    for(int i = 0 ; i <sizeofarray;i++)
                    {
                        file.write((char*)temp,1);
                        filesize-=1;
                    }
                    temp++;
            
                }
            }
            file.close();
            delete[] mBlock;
          
        }
        else
        {
            cerr<<"File could not be opened while decompressing 2!"<<endl;
        }
        
    }
    else
    {
        cerr<<"File could not be opened to decompress!"<<endl;
    }
    clock_t end = clock();
    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nDecompressing took " << elapsed_time << " seconds." << endl;   

}

int string2int(const string &s)
{
    int ans = 0; // variable for the result
    for (size_t i = 0; i < s.length(); i++)
    {
        ans *= 10; // multiply the result by 10 at each step to define the proper digit
        ans += s[i] - '0'; // transforms a numerical char into int and adds it to the result
    }
    return ans; // return the converted number
}

class CompressedFile
{
    private:
    string name;
    string header;
    string comment;
    int size;
    unsigned short height;
    unsigned short width;
    unsigned char** mat;
    
    public:
    //Constructors
    CompressedFile();
    CompressedFile(string name);
    ~CompressedFile();

    //Setters getter
    string getHeader(void);
    string getComment(void);
    string getName();
    int getSize(void);
    int getHeight(void);
    int getWidth(void);
    
    //Functions
    void Compressor(int offset = 20);
    
    
};

CompressedFile::CompressedFile()
{
    cout << "Default Constructer called!" << endl;
    this->name = "NULL";
    this->header = "NULL";
    this->comment = "NULL";
    this->height = 0;
    this->width = 0;
    this->size = 0;
}

CompressedFile::CompressedFile(string name)
{
    
    this->name = name;
    string nameoffile = name + ".pgm";
    this->width = 0;
    this->height = 0;
    string temp = "";
    string line;
    ifstream file(nameoffile, ios::binary|ios::ate);
    if(file.is_open())
    {
        streampos fileSize = file.tellg();
        unsigned char* mBlock = new (nothrow) unsigned char[fileSize];
        
        if(mBlock != nullptr)
        {
            file.seekg(0,ios::beg);
            file.read((char*)mBlock,fileSize);
            file.close();
            
            unsigned char* p = mBlock;

            // -- Gets the type of P (P5,P2) -- //
            this->header += *(p++); 
            this->header += *(p++);
            this->header += *(p++);
            fileSize-=3;

            // ----------------------------------------------------------------------------------------- //
            //                Reads comments and assigns it into comment member                          //
            // ----------------------------------------------------------------------------------------- //

            while(true)
            {
                if (*p == '#') // -- Skips the # at the comment block -- //
                {
                    p++;
                    fileSize -=1;
                    while(*p != '\n') // -- Write the comment in to the comment member until it sees a new line -- //
                    {
                        this->comment += *p;
                        p++;
                        fileSize -=1;
                    }
                }
                
                break;
            }
            // -- When comment finishes, go to the next line -- //
            

            // -- Get the resolution of the file -- //
            
            while(*p != ' ') // -- String to integer to get the value of the width. -- //
            {
                if(!(isalnum(*p)))
                {
                    p++;
                    fileSize-=1;
                }
                else
                {
                    temp += *p;
                    this->header += *p;
                    p++; 
                    fileSize -= 1;
                }
            }
            this->width = string2int(temp);

            // -- After reaching space character, write the space into the header -- //

            this->header += *p; 
            p++;
            fileSize -=1;

            temp = ""; // -- Resetting temp string to get the value of height -- //

            while(*p != '\n') // -- String to integer to get the value of the height. -- //
            {

                temp += *p;                
                this->header += *p;
                p++; 
                fileSize -= 1;
                
            }
            this->height = string2int(temp);
            temp = "";
            

            // -- After reaching space character, write the space into the header -- //

            this->header += '\n';
            p++; 
            fileSize -= 1; 

            // -- Range of the pixels -- //
            while (*p != '\n')
            {
                this->header += *p;
                p++; 
                fileSize -= 1; 
            }
            p++; 
            fileSize -= 1; 
            this->size = this->width*this->height;
            
            
            // ----------------------------------------------------------------------------------------- //
            //     End of reading the comments, header and assigning them to the corresponding members   //
            // ----------------------------------------------------------------------------------------- //

            // ----------------------------------------------------------------------------------------- //
            //     Creating a matrix composed of pixels of the image in type of characters               // 
            // ----------------------------------------------------------------------------------------- //

            //Note: Because the pixels are composed of 1 byte so a size of a character we are creating a matrix that holds characters.
            
            
            // -- Creating the matrix based off of the resolution of the image.-- //
            this->mat = new(nothrow) unsigned char*[this->height];
            unsigned char* arr = new unsigned char[this->size];
            for(int i=0; i<this->height;i++)
            {
                mat[i] = &arr[i*this->width];
            }
            
            if(mat != nullptr)
            {
                
                while(fileSize > 0)
                {   
                    for(int i = 0; i< this->height; i++) 
                    {
                        for(int j = 0;j<this->width;j++)
                        {
                            mat[i][j] = *p;
                            p++;
                            fileSize -= 1;
                            
                        }
                    }
                }
            }
    
        }
        delete[] mBlock;
        mBlock = nullptr;
    }

    // ----------------------------------------------------------------------------------------- //
    //                                 End of creating the matrix                                //
    // ----------------------------------------------------------------------------------------- //

    else
    {
        cerr<<"File could not be opened!"<<endl;
    }

}

    // ----------------------------------------------------------------------------------------- //
    //                                Starting to compress the file                              //
    // ----------------------------------------------------------------------------------------- //



void CompressedFile::Compressor(int offset)
{
    clock_t start = clock();
    // -- Offset represents the compression amount of the image -- //

    if(offset <= 0 || offset >= 200)
    {
        offset = 20;
    }
    string path = this->name + ".ezy";
    ofstream file(path, ios::binary);
    // -- While compressing, we are writing the width and the height of the image before everything else in to the file -- // 
    file.write((char*)&this->width, sizeof(unsigned short));
    file.write((char*)&this->height, sizeof(unsigned short));
    if(file.is_open())
    {
        unsigned char color;
        int counter = 0;
        int avg = 0;
        int k = 0;
        int l = 0;

        // -- We iterate over the rows and columns, and write into the e.ezy. -- //
        while(k < this->height)
        {
            // -- First we get a color from the matrix to count the iterations -- //
            color = this->mat[k][l];
            int min = 0;
            int max = 255;

            // -- To prevent getting an underflow or overflow, we check certain conditions and then modify the min and the max values -- //

            if(offset <= color)
            {
                min = color-offset;
            }

            unsigned short overflow = offset + color; // -- We use unsigned short variable to check an overflow. Because offset is an integer andd color is unsigned char. -- //
            if(overflow < max)
            {
                max = color+offset;
            }


            // -- This is where the actual compressing is getting done -- //
            // -- The compressing works like this ->
            // -- After getting a color from the matrix, we check the matrix row-wise for closest colors in the range of the min and max,
            //  after getting the number of closest colors in the range of the offset from the matrix, we get their average number,
            // write it into the matrix in such order = the count of the color, color. Eg. 65 03 meaning when decompressing, write the color 03 65 times. -- //
            
            while((min<= this->mat[k][l]) && (this->mat[k][l] <= max))
            {
                counter++;
                avg += mat[k][l];
                l++;
                if(l == this->width)// -- If end of the columns go down -- //
                {
                    k++;
                    if(k == this->height) // -- If end of the row exit -- //
                    {
                        break;
                    }
                    else
                    {
                        l = 0;
                    }
                }
                if(counter == 255) // -- Unsigned char's max value is 255. If it exceeds the 255 it will start to faultily compress. So this condtion prevents it. -- //
                {
                    break;
                }
            }

            // -- After while loop, it takes average scale of the pixels counted and writes counter as unsigned char and average as unsigned char into the .ezy file. -- //
            avg = avg/counter;
            file.write((char*)&counter,1);
            counter = 0;
            file.write((char*)&avg,1);
            avg = 0;
        }
        
        file.close();
        
    }
    else
    {
        cerr<<"File could not be created!"<<endl;
    }
    
    // -- Clock library is to seem cool -- //
    clock_t end = clock();
    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nCompressing took " << elapsed_time << " seconds." << endl;
}

    // ----------------------------------------------------------------------------------------- //
    //                                End of  compression                                        //
    // ----------------------------------------------------------------------------------------- //

CompressedFile::~CompressedFile()
{
    if(this->mat != nullptr)
    {
        delete[] this->mat[0];
        delete[] this->mat;
        this->mat = nullptr; 
    }
}
string CompressedFile::getName(void)
{
    return this->name;
}
int CompressedFile::getHeight(void)
{
    return this->height;
}
int CompressedFile::getWidth(void)
{
    return this->width;
}
int CompressedFile::getSize(void)
{
    return this->size;
}
string CompressedFile::getHeader(void)
{
    return this->header;
}
string CompressedFile::getComment(void)
{
    return this->comment;
}