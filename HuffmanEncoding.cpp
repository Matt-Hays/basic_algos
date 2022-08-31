/**
 * @file HuffmanEncoding.cpp
 * @author Matthew Hays
 * @brief Huffman Encoding and Decoding
 * @date 2022-07-31
 * 
 * This program accepts file paths as inputs and generate an encoded message of the input file's contents.
 * The program can also take a file that has been encoded using this algorithm and output a decoded version of the contents
 * a separate output file.
 * 
 * Time Complexity: 
 * 
 * The traversal of the huffman tree, traversal of the minimum heap, and read/write operations on the file contents control the 
 * time complexity of the program.
 * 
 * For traversal of the Huffman tree: O(log n) Worst case when the tree is not balanced => O(n).
 * For sorting of the minimum heap: O(n logn).
 * For read write operations: O(n).
 * 
 * Therefore, the time complexity of this program is dominated by sorting the minimum heap to
 * ensure that we always retrieve a minimum value. 
 * 
 * O(n logn) with worst case O(n logn).
 * 
 * Usage: ./huffman --huff <source> <destination>
 *        ./huffman --unhuff <source> <destination>
 * 
 * NOTE: This program can only "--unhuff" a file that was generated using this program's "--huff" option.
 */

#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <string>

using namespace std;

/**
 * @brief Node Structure for use in our trees.
 * 
 */
struct Node {
    int freq;
    char data;

    Node *left, *right;

    Node(char data, int freq){
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

/**
 * @brief Provides a method for comparison for our STL minimum heap.
 * 
 */
class NodeComparator {
    public:
        int operator() (Node *node1, Node *node2){
            return node1->freq > node2->freq;
        }
};

/**
 * @brief Traverse the Huffman Tree and generate a hashmap of encoded values for each character.
 * 
 * @param node The root node of the Huffman Tree.
 * @param binaryRepresentation Initially an empty string, used to generate the output encodings.
 * @param encodingMap The hashmap that will contain our encodings.
 */
void generateEncodingMap(Node *node, string binaryRepresentation, map<char, string> &encodingMap){
    if(!node) return;
    if(node->data != '$')
        encodingMap[node->data] = binaryRepresentation;
    
    generateEncodingMap(node->left, binaryRepresentation + "0", encodingMap);
    generateEncodingMap(node->right, binaryRepresentation + "1", encodingMap);
}

/**
 * @brief Given a file path to an unencoded message, write a huffman tree representation
 * and the encoded message to a user specified output file.
 * 
 * @param filePath The input file path.
 * @return string 
 */
void huff(string filePath, string outputFilePath){
    ifstream file(filePath);
    map<char, int> freqCtr;
    priority_queue<Node*, vector<Node*>, NodeComparator> minimumHeap;
    string tempStr, encodedMessage, unencodedMessage;
    int lineCounter = 0;

    // Gather the unecoded message from the input file.
    if(!file.is_open()) throw invalid_argument("File not open.");
    while(getline(file, tempStr)){
        if(lineCounter == 0)
            unencodedMessage += tempStr;
        else
            unencodedMessage += "\n" + tempStr;
        lineCounter++;
    }
    file.close();

    // Generate a frequency counter for the frequency the character appears in the message.
    for(int i = 0; i < unencodedMessage.length(); i++){
        if(freqCtr[unencodedMessage.at(i)])
            freqCtr[unencodedMessage.at(i)]++;
        else
            freqCtr[unencodedMessage.at(i)] = 1;
    }

    // Add each key in the freqCtr as a Node in the minimum heap.
    map<char, int>::iterator freqItr;
    for(freqItr = freqCtr.begin(); freqItr != freqCtr.end(); freqItr++)
        minimumHeap.push(new Node(freqItr->first, freqItr->second));
    
    // Build the Huffman Tree
    Node *left, *right, *top;
    while(minimumHeap.size() >= 2){
        left = minimumHeap.top();
        minimumHeap.pop();
        right = minimumHeap.top();
        minimumHeap.pop();
        top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minimumHeap.push(top);
    }

    // Generate a hashmap of characters and values for fast loopup during encoding.
    map<char, string> encodingMap;
    generateEncodingMap(minimumHeap.top(), "", encodingMap);


    // Convert the unencoded message to an encoded message using the hashmap.
    for(int i = 0; i < unencodedMessage.length(); i++){
        encodedMessage += encodingMap[unencodedMessage.at(i)];
    }


    // Find the size needed for the array
    map<char, string>::iterator encodItr;
    int indexLocation = 0;
    int maxIndex = 0;
    for(encodItr = encodingMap.begin(); encodItr != encodingMap.end(); encodItr++){
        char key = encodItr->first;
        string binaryRep = encodItr->second;
        // Determine index location to place the character.
        for(int i = 0; i < binaryRep.length(); i++){
            if(binaryRep.at(i) == '0')
                indexLocation = 2 * indexLocation + 1;
            else if(binaryRep.at(i) == '1')
                indexLocation = 2 * indexLocation + 2;
        }
        if(indexLocation > maxIndex) maxIndex = indexLocation; 
        // Place the character in the array.
        indexLocation = 0;
    }
    // We need an aray of size 2 * number of chars - 1 to hold chars.
    char arrayRepresentationOfTree[maxIndex + 1];

    // Initialize the array to all nulls.
    for(int i = 0; i < sizeof(arrayRepresentationOfTree); i++){
        arrayRepresentationOfTree[i] = '$';
    }

    // Using the map, place the keys at calcualted values:
    // a 0 indicates 2*n+1 (left child)
    // a 1 indicates 2*n+2 (right child)
    int indexLocation2 = 0;
    map<char, string>::iterator encodItr2;
    for(encodItr2 = encodingMap.begin(); encodItr2 != encodingMap.end(); encodItr2++){
        char key = encodItr2->first;
        string binaryRep = encodItr2->second;
        // Determine index location to place the character.
        for(int i = 0; i < binaryRep.length(); i++){
            if(binaryRep.at(i) == '0')
                indexLocation2 = 2 * indexLocation2 + 1;
            else if(binaryRep.at(i) == '1')
                indexLocation2 = 2 * indexLocation2 + 2;
        }
        // Place the character in the array.
        arrayRepresentationOfTree[indexLocation2] = key;
        indexLocation2 = 0;
    }

    ofstream outputFile(outputFilePath, ios::out | ios::binary);

    // Write identifying data.
    char identifier = 'H';
    outputFile.write((char*) &identifier, sizeof(char));

    // Write size of decoded message.
    int sizeOfOrigMessage = unencodedMessage.length();
    outputFile.write((char*) &sizeOfOrigMessage, sizeof(int));

    // Write the tree array and metadata to the binary file.
    for(int i = 0; i < sizeof(arrayRepresentationOfTree); i++){
        outputFile.write((char*) &arrayRepresentationOfTree[i], sizeof(char));
    }
    // // Write a end of header identifier.
    char newLine = '\\';
    outputFile.write((char*) &newLine, sizeof(char));

    // Write the encoded message to its own binary file.
    int bitCounter = 7;
    unsigned char bitBuffer;
    string buffer;
    for(int i = 0; i < encodedMessage.length(); i++){
        buffer += encodedMessage.at(i);
        if(buffer.length() == 8){
            bitset<8> insertBuffer(buffer);
            outputFile.write((char*) &insertBuffer, sizeof(char));
            buffer = "";
        }
        if(i == encodedMessage.length() - 1 && buffer.length() != 8){
            int diff = 8 - buffer.length();
            for(int i = 0; i < diff; i++){
                buffer += '0';
            }
            bitset<8> insertBuffer2(buffer);
            outputFile.write((char*) &insertBuffer2, sizeof(char));
            buffer = "";
        }
    }
    outputFile.close();
}

/**
 * @brief Given a file path to an encoded file that was generated via this program's --huff command
 * and a file path to the desired output file location, decode a given file.
 * 
 * @param filePath The path to the file which was encoded using this program.
 * @param outputFilePath The file path to the desired output location.
 */
void unhuff(string filePath, string outputFilePath){
    ifstream inputFile(filePath, ios::in | ios::binary);
    vector<char> huffmanTreeVectorRep;

    // Check if the encoded file was encoded by this algorithm.
    char identity;
    inputFile.read((char*) &identity, sizeof(char));
    if(identity != 'H') return;

    // Get the decoded message size.
    int messageSize;
    inputFile.read((char*) &messageSize, sizeof(int));

    char tempCharacter;
    int newLineCounter = 0;
    while(tempCharacter != '\\') {
        inputFile.read((char*) &tempCharacter, sizeof(char));
        huffmanTreeVectorRep.push_back(tempCharacter);
    }

    // Read in encoded message.
    char tempChar;
    string messageBitRepresentation;
    while(inputFile.get(tempChar)){
        messageBitRepresentation += bitset<8>(tempChar).to_string();
    }
    inputFile.close();

    // Decode the message
    string decodedMessage;
    int currIdx = 0;
    for(int i = 0; i < messageBitRepresentation.length(); i++){
        if(decodedMessage.length() >= messageSize) break;
        if(messageBitRepresentation.at(i) == '0'){
            // Move left
            currIdx = 2 * currIdx + 1;
            if(huffmanTreeVectorRep.at(currIdx) != '$'){
                decodedMessage += huffmanTreeVectorRep.at(currIdx);
                currIdx = 0;
            }
        }
        else if (messageBitRepresentation.at(i) == '1'){
            // Move right
            currIdx = 2 * currIdx + 2;
            if(huffmanTreeVectorRep.at(currIdx) != '$'){
                decodedMessage += huffmanTreeVectorRep.at(currIdx);
                currIdx = 0;
            }
        }
    }
    cout << decodedMessage << endl;

    ofstream outputFile(outputFilePath);
    outputFile << decodedMessage;
    outputFile.close();
}

int main(int argc, char* argv[]){

    string command = argv[1];
    string inputFilePath(argv[2]);
    string outputFilePath(argv[3]);
    string encodedMessage;

    if(argc != 4) return 1;

    if(command == "--huff")
        huff(inputFilePath, outputFilePath);
    else if(command == "--unhuff")
        unhuff(inputFilePath, outputFilePath);


    return 0;
}