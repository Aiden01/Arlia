#include "compile.hpp"

void Compile::SetFilenames() {
	std::string WithoutExtention = System::File::WithoutExtention(SourceFile);
	OutputAsm = WithoutExtention + ".asm";
	OutputExe = WithoutExtention + ".exe";
}
std::string Compile::FileSize(std::string filename) {
	return System::Text::DeleteUnnecessaryZeros(std::to_string(System::File::GetFileSize(filename)));
}
Compile::Compile(std::string filename) {
	std::remove(System::Text::StringToCharArray(LogMessage::LogFile));
	std::remove(System::Text::StringToCharArray(OutputExe));
	SourceFile = filename;
	SetFilenames();
}
void Compile::run() {
	tclock = clock();
	lexer.start(SourceFile);
	parser.parse(lexer, HeaderImported);
	time = tclock;
	lexer.end();
}
void Compile::write() {
	if (!HasErrors()) System::File::write(OutputAsm, code.GetAsm());
	else LogMessage::LogMessage("Unable to compile due to errors", true);
}
bool Compile::HasErrors() {
	return LogMessage::NbrOfErrors;
}
double Compile::GetTime() {
	return time;
}
bool Compile::IsSuccess() {
	return System::File::exist(OutputExe);
}
void Compile::DisplayCompilationInformation() {
	std::cout << std::endl;
	LogMessage::LogMessage("Original file size: " + FileSize(SourceFile) + " bytes", false);
	if (IsSuccess()) {
		LogMessage::LogMessage("Compilation success!");
		LogMessage::LogMessage("Asm output file size: " + FileSize(OutputAsm), false);
		LogMessage::LogMessage("Exe output file size: " + FileSize(OutputExe), false);
	}
	else LogMessage::LogMessage("Compilation non success.");
	std::cout << std::endl << "Time taken: " <<
		System::Text::DeleteUnnecessaryZeros(std::to_string((clock() - time) / CLOCKS_PER_SEC)) << "s." << std::endl;
}
