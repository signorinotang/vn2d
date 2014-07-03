#include "vnApplication.h"
#include "vnLog.h"
#include "vnFileManager.h"
#include "../src/win/Game.h"

int main() {
	

	vn::Log::instance().addPrinter(vnnew vn::LogPrinterSTDOUT(), false);
	vn::FileManager::instance().setFileSystem(1, vnnew vn::FileSystemOS("C:\\Users\\tanghailong\\projects\\works new\\vn2d\\prj\\test\\"), false);

	return vn::Application::main(&vn::Game::instance(), true);
}