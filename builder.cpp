#include <boost/program_options.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <chrono>
using namespace std;

boost::process::child Build(int clock = 0)
{
	string cmake = "cmake --build _builds";
	cout << "Command = " << cmake << endl;
	boost::process::child process(cmake, boost::process::std_out > stdout);
	if (clock != 0)
	{
		if (!process.wait_for(chrono::seconds(clock)));
		process.terminate();
	}
	else process.wait();
	return process;
}

boost::process::child Target(string install)
{
	string cmake = "cmake --build _builds --target " + install;
	cout << "Command = " << cmake << endl;
	boost::process::child process(cmake, boost::process::std_out > stdout);
	process.wait();
	return process;
}

boost::process::child Make(int clock = 0, string configuration = "Debug")
{
	string cmake = "cmake -H. -B_build -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=" + configuration;
	cout << "Command : " << cmake << endl;
	boost::process::child process(cmake, boost::process::std_out > stdout);
	if (clock != 0)
	{
		if (!process.wait_for(chrono::seconds(clock)));
		process.terminate();
	}
	else process.wait();
	return process;
}

int main(int argc, char *argv[])
{
	boost::program_options::options_description options("Allowed options");
	options.add_options()
		("help", "выводим вспомогательное сообщение")
		("config", boost::program_options::value<string>(), "указываем конфигурацию сборки (по умолчанию Debug)")
		("install", "добавляем этап установки (в директорию _install)")
		("pack", "добавляем этап упакови\n(в архив формата tar.gz)")
		("timeout", boost::program_options::value<int>(), "указываем время ожидания (в секундах)");
	boost::program_options::variables_map varMap;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), varMap);
	boost::program_options::notify(varMap);
	int code;
	if (varMap.count("help")) cout << options << endl;
	else if (varMap.count("config"))
	{
		string par(varMap["config"].as<string>());
		code = Make(0, par).exit_code();
		if (code == 0) code = Build().exit_code();
		cout << "Code : " << code << endl;
	}
	else if (varMap.count("pack"))
	{
		code = Make().exit_code();
		if (code == 0) code = Build().exit_code();
		if (code == 0) code = Target("package").exit_code();
		cout << "code == " << code << endl;
	}
	else if (varMap.count("install"))
	{
		code = Make().exit_code();
		if (code == 0) code = Build().exit_code();
		if (code == 0) code = Target("install").exit_code();
		cout << "Code : " << code << endl;
	}
	else if (varMap.count("pack") && varMap.count("install"))
	{
		code = Make().exit_code();
		if (code == 0) code = Build().exit_code();
		if (code == 0) code = Target("install").exit_code();
		if (code == 0) code = Target("pack").exit_code();
		cout << "Code : " << code << endl;
	}
	else if (varMap.count("timeout"))
	{
		int time = varMap["timeout"].as<int>();
		code = Make(time).exit_code();
		if (code == 0) code = Build(time).exit_code();
		cout << "code == " << code << endl;
	}
	else
	{
		code = Make().exit_code();
		if (code == 0) code = Build().exit_code();
		cout << "Code : " << code << endl;
	}
	system("pause");
	return 0;
}