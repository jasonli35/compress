#include "Helper.hpp"
#include "HCTree.hpp"

int main(int argc, char* argv[]){
    if(argc != 3){
        error("Incorrect parameters\nUSAGE: ./refcompress <original_file> <compressed_file>");
    }
    FancyInputStream istream(argv[1]);
    FancyOutputStream oStream(argv[2]);
    
    if(istream.filesize() == 0){
        return 0;
    }
    if(!istream.good()){
        string message = "ERROR: File not found: ";
        message.append(argv[1]);
        cerr << message << endl;
        
    }

    int index;

    vector<unsigned short int> counts = vector<unsigned short int>(256, 0);
    do{
        index = istream.read_byte();
        if(index != -1){
            counts[index] = counts[index] + 1;
        }
    }while(index != -1);

    HCTree hctree = HCTree();
    hctree.build(counts);

    
    for(long unsigned int i = 0; i < counts.size(); i++){
     
        unsigned short int intToWrite = counts[i];
        oStream.write_byte(intToWrite >> 8);
        intToWrite = counts[i];
        oStream.write_byte(intToWrite & 255);  //not sure
    }

    istream.reset();
    do{
        index = istream.read_byte();
        if(index != -1){
            hctree.encode((unsigned char) index, oStream);
        }
    }while(index != -1);







    return 0;





}

