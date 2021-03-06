#include "stdafx.h"
#include "Includes.h"
	

//
// Start Defintions Global

bool swit = false;

// End Definitions Global
//
// Start Definitions Files 

string fName;
char szFile[1000];
OPENFILENAME files;
std::vector<std::string> listOfFiles;

// End Defintions Files
//
// Start Definitions Misc

string answer;
bool comments = false;
bool emptylines = false;
int geslines = { 0 };
string ignorefiles;

// End Definitions Misc
//




//Öffnet einen Filedialog, in welchem eine Datei ausgewählt werden kann.
void Files() 
{

	
		ZeroMemory(&files, sizeof(files));
		files.lStructSize = sizeof(files);
		files.lpstrFile = szFile;
		files.lpstrFile[0] = '\0';
		files.nMaxFile = sizeof(szFile);
		files.Flags = OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_FILEMUSTEXIST;
		GetOpenFileName(&files);
		fName = files.lpstrFile;
		std::experimental::filesystem::path path(fName);
		
		if (swit) // Wenn Ordner ausgewählt, ermittelt er den Path(parent_path) von der ausgewählten Datei.
		{
			filesys::path p = fName;
			path = p.parent_path(); 
			fName = path.string();
		}
}


void out(string str) // Verkürzung um sich das dauernde Cout, [>] und Zeilenumbruch zu sparen.
{

	std::cout << "[>] " << str.c_str() << " \n";

}

void outwonl(string str) // Genau wie out, nur ohne Zeilenumbruch.
{

	std::cout << "[>] " << str.c_str();
}

inline std::string& ltrim(std::string& str, const char* t = " \t\n\r\f\v") // Entfernt Whitespaces links
{
	str.erase(0, str.find_first_not_of(t));
	return str;
}


inline std::string& rtrim(std::string& str, const char* t = " \t\n\r\f\v")  // Entfernt Whitespaces rechts
{
	str.erase(str.find_last_not_of(t) + 1);
	return str;
}


int GetLines(const std::string& filename)	// Ite. durch die Lines
{

	bool MultiLineComment = false;	
	int lines = 0;

	std::ifstream infile(filename); // Öfnet Datei
	for (std::string line; getline(infile, line); )
	{

		line = ltrim(rtrim(line)); // L&R-Trim

		if (emptylines) // Ignoriert leere Zeilen
			{
				if (line.empty())
					{
						continue;
					}
			}

		if (comments == true) //Ignoriert Kommentare
			{
				if (MultiLineComment)
					{
						if (line.find("*/") == 0)
							{	MultiLineComment = false;	}
						else
							{	continue;	}
					}
			
				
				if (line.find("//") == 0)
					{	continue;	}


				if (line.find("/*") == 0 || line.find("/**") == 0)
					{
						MultiLineComment = true;
						continue;
					}

			}


		lines++;	//Erhöht den Counter für die Zeilen einer einzelnen Datei
		geslines++;	//Erhöht den Counter für alle Dateien
		
	}
	
	cout << " - Es sind: " << lines << " Zeilen." << "\n" << endl;

}




std::vector<std::string> getAllFilesInDir(const std::string &dirPath, const std::vector<std::string> dirSkipList = {})
{

	try {
		
		
		if (filesys::exists(dirPath) && filesys::is_directory(dirPath))	// Überprüft ob Path valid und ob Ordner
			{
				
				filesys::recursive_directory_iterator iter(dirPath);	// Iter. 
				filesys::recursive_directory_iterator end;

				while (iter != end)
					{
					
						if (filesys::is_directory(iter->path()) &&
						(std::find(dirSkipList.begin(), dirSkipList.end(), iter->path().filename()) != dirSkipList.end()))
							{	iter.disable_recursion_pending();	}
						
						else
							{	listOfFiles.push_back(iter->path().string());	}

				error_code ec;
				iter.increment(ec);
				if (ec) {
					std::cerr << "[>] Fehler bei Zugriff auf: " << iter->path().string() << " - " << ec.message() << '\n';
				}
			}
		}
	}
	catch (std::system_error & e)	// Exceptionhandling
	{
		std::cerr << "[>] Exception: " << e.what();
	}
	return listOfFiles;
}






int main()
{
	
	SetConsoleTitle(_T("Line-Counter - NAME geändert weil GitRepo"));

	out("Line-Counter - NAME geändert weil GitRepo");
	out("Wenn ein Ordner ausgew\x84hlt wird, werden alle Datein und Unterordner miteinbezogen");

Question:

	geslines = 0;

	out("Sollen Kommentare gefiltert werden? y/n");

	cin >> answer;

	if (answer == "y")		//Kommentarcheck
		{	comments = true;	}
	else if (answer == "n")
		{	comments = false;	}
	else
		{ 
			system("cls");
			std::cout << answer.c_str() << " ist keine valide Eingabe" << std::endl;
			out(" Es werden nur y oder n als Antwort akzeptiert.");
			goto Question;
		}


	out("Sollen leere Zeilen gefiltert werden? y/n");

	cin >> answer;

	if (answer == "y")		//Emptylinecheck
		{
			emptylines = true;
		}
	else if (answer == "n")
		{
			emptylines = false;
		}
	else
		{
			system("cls");
			std::cout << answer.c_str() << " ist keine valide Eingabe" << std::endl;
			out(" Es werden nur y oder n als Antwort akzeptiert.");
			goto Question;
		}


	out("Datei oder Ordner? d/o");
	cin >> answer;
	if (answer == "d")		//Select dir/file
		{
			swit = false;
		}
	else if (answer == "o")
		{
			out("Einfach eine Datei im entsprechenden Ordner ausw\x84hlen und dieser sowie Unterordner werden ber\x81 cksichtigt.");
			swit = true;
		}
	else
		{ 
			system("cls");
			std::cout << answer.c_str() << " ist keine valide Eingabe" << std::endl;
			out(" Es werden nur d oder o als Antwort akzeptiert.");
			goto Question;
		}


	Files();	//ruft den Filedialog auf


	if (swit)	// Wenn Ordner
		{
			int ges = { 0 };
			out("Soll eine Endung ignoriert werden? y/n");
			cin >> answer;
			if (answer == "y")
			{
				out("Bitte Endung eingeben:");
				cin >> ignorefiles;
				std::vector<std::string> list = getAllFilesInDir(fName, { ignorefiles });
			}
			else if (answer == "n")
				{
					std::vector<std::string> list = getAllFilesInDir(fName);
				}

			for (auto str : listOfFiles)
				{
					ges++;
					cout << str << GetLines(str) << "  ";
				}
		
			cout << "[>] Insgesamt " << ges << " Dateien wurden durchsucht \n";
			cout << "[>] Gesamte Zeilen: " << geslines << "\n";
		}
	else {	GetLines(fName);	}	//Wenn Datei


			

Question2:

	out("Vorgang mit neuer Datei wiederholen? y/n");	// Jump zum Anfang
	cin >> answer;

	if (answer == "y")
		{
			goto Question;
		}
	else if (answer == "n")
		{
			return 0;
		}
	else 
		{
			system("cls");
			out("Die Eingabe war leer oder nicht valid.");
			goto Question2;
		}


}

