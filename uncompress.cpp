#include "Helper.hpp"
#include "HCTree.hpp"


int main(int argc, char* argv[]){
    if(argc != 3){
        error("Incorrect parameters\nUSAGE: ./refcompress <original_file> <compressed_file>");
    }


    vector<unsigned short int> counts = vector<unsigned short int>(256, 0);
    FancyInputStream inStream(argv[1]);
    FancyOutputStream outStream(argv[2]);

    if(inStream.filesize() == 0){
        return 0;
    }
        
    if(!inStream.good()){
        string message = "ERROR: File not found: ";
        message.append(argv[1]);
        cout << message << endl;
        exit(1);
        
    }

    

    int count = 0;

    for(int i = 0; i < 256; i++){
        unsigned short int currCount = inStream.read_byte() << 8;
        currCount = currCount | inStream.read_byte();
        counts[i] = currCount;
        count += currCount;
    }
    HCTree tree;
    tree.build(counts);

    for(int i = 0; i < count; i++){
        outStream.write_byte(tree.decode(inStream));
    }
        
        

    return 0;


    
    
}
