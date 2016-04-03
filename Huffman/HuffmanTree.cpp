#include "HuffmanTree.h"
#include <iostream>
#include <sstream>
#include <bitset>
void HuffmanTree::huffmanEncoding(TreeNode * ptr, string direction)
{
	// Improved on 
	if (!ptr) // empty tree root is Null
	{
		cout << "Empty Tree" << endl;
	}
	else
	{
		if (ptr->data == NULL)
		{
			huffmanEncoding(ptr->leftPtr, direction + '0');
			huffmanEncoding(ptr->rightPtr, direction + '1');
		}
		else
		{
			huffMap.insert(pair<char, std::string>(ptr->data, direction));

		}
	}
}




string HuffmanTree::readFile(string fileIn)
{
	// reads the txt file. Maybe allow user input?
	//string input;
	ifstream originalMessage(fileIn);
	// while there is a line available get it.
	if (originalMessage.is_open()) {
		while (originalMessage)
		{
			getline(originalMessage, message);

		}
	}
	else
	{
		cout << "File Not Found" << endl;
	}
	originalMessage.close(); // close the ifstream
	return message;
}

void HuffmanTree::writeToFile(string textInput, string fileToWrite)
{
	ofstream outputFile;
	outputFile.open(fileToWrite);
	outputFile << textInput;
	outputFile.close();
}

void HuffmanTree::frequencyTable()
{
	for (int i = 0; i < message.length(); i++)
	{
		// letter not found add it with frequency of one
		if (frequencyMap.find(message[i]) == frequencyMap.end())
		{
			frequencyMap.insert(pair<char, int>(message[i], 1));
		}
		// letter exists then its frequency +1
		else
		{
			frequencyMap.find(message[i])->second++; // NOTE : "second is the number value"
		}
	}
}

void HuffmanTree::displayFreqTable()
{
	// test data went into freq map
	// Frequency of the tree is 26.
	// Frequency of the tree verified at www.algorasim.com
	map<char, int> ::iterator freqItr;

	cout << "Frequency Map" << endl << "----------------------- " << endl;
	for (freqItr = frequencyMap.begin(); freqItr != frequencyMap.end(); freqItr++)
	{
		cout << freqItr->first << "  " << freqItr->second << endl;
	}
	cout << endl;
}


void HuffmanTree::buildHeap()
{
	// Populate the priority queue
	map<char, int> ::iterator pQ;
	for (pQ = frequencyMap.begin(); pQ != frequencyMap.end(); pQ++)
	{
		heap.emplace(new TreeNode(pQ->first, pQ->second)); // push new anonymous node into the priority queue
														   //cout << pQ->first << " " << pQ->second << endl;
	}
	// build the Huffman tree
	// Note: George told me about the emplace();
	while (heap.size() != 1)
	{
		TreeNode *leftSide = new TreeNode(heap.top());// store the top node from the heap
													  //cout << heap.top()->data << " " << heap.top()->frequency << endl;
		heap.pop(); // pop off the top node from the heap (the one above)
		TreeNode *rightSide = new TreeNode(heap.top());
		//cout << heap.top()->data << " " << heap.top()->frequency << endl;
		heap.pop();
		TreeNode *newNode = new TreeNode(leftSide, rightSide);
		heap.emplace(newNode);  // push new node (tree), and add up the frequency (populated in PQ based on frequency)
								//cout << heap.top()->data<<" "<< heap.top()->frequency << endl;
	}
	root = heap.top(); // ref(set) the last node as the root
					   //cout << heap.top()->data << " " << heap.top()->frequency << endl; // debugging
	heap.pop(); // pop off the tree from the priority queue
}

void HuffmanTree::huffmanEncoding()
{
	huffmanEncoding(root, "");
	displayHuffmanTable();
}

void HuffmanTree::displayHuffmanTable()
{
	cout << "Huffman Table:" << endl << "----------------------- " << endl;
	map<char, string> ::iterator itr;
	for (itr = huffMap.begin(); itr != huffMap.end(); itr++)
	{
		cout << itr->first << " " << itr->second << endl;
	}
	cout << endl;
}

void HuffmanTree::writeHuffCodeToFile()
{

	// take the input text (The one in main), loop through it, find respective Huffman code in the map and add to the string
	for (int i = 0; i < message.size(); i++) {
		code += huffMap.find(message[i])->second;

	}
	writeToFile(code, "huffmanCode.txt");
}

void HuffmanTree::decodeHuffFile()
{
	// read from the file that contains the binary without padding
	code = readFile("huffmanCode.txt");
	string decodedHuffman = huffDecode(code);
	// write out the decoded message to file
	writeToFile(decodedHuffman, "DecodedHuffman.txt");

	// printing on screen the decode message should be same as the original message
	cout << "The decoded message should be the same as original \n" << endl;
	cout << decodedHuffman << endl;
	cout << endl;
}

// Taken out of the decodeHuffFile() to make it easier to debug
// Makes it easier to read the code too.
string HuffmanTree::huffDecode(string codeIn)
{
	// similar to the encoding 
	string decodedHuffman = ""; // to store the decoded text
	TreeNode* nodePtr = root;
	for (int i = 0; i != codeIn.size() - padding; i++)
	{
		if (codeIn[i] == '0') { // if 0, go left
			nodePtr = nodePtr->leftPtr;
		}
		else { // else, it can only be 1, which means go right
			nodePtr = nodePtr->rightPtr;
		}
		// leaf node
		if (nodePtr->leftPtr == nullptr && nodePtr->rightPtr == nullptr)
		{
			// append the letter in that leaf node to the string
			decodedHuffman += nodePtr->data;
			nodePtr = root; // reset node pointer to root
		}
	}
	return decodedHuffman;
}

