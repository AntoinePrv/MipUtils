#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include "../lib/coefs.hpp"

namespace po = boost::program_options;


po::variables_map process_program_options(
	const int argc,
	const char *const argv[]
){
	po::options_description desc(
		"\nMIP coefficients extractor.\n\n"
		"Usage: mip-coefs [options] input-file [output-file]\n"
		"Allowed arguments");

	desc.add_options()
		("help,h", "Produce this help message.")
		("input-file,i", po::value<std::string>()->required(),
		 "Specifies input file.")
		("sparse,s", "Write solution in sparse format.")
		("output-file,o", po::value<std::string>(),
		 "Specifies output file for the coefficients.");

	po::positional_options_description p;
	p.add("input-file", 1);
	p.add("output-file", 1);

	po::variables_map vm;
	try {
		po::store(
			po::command_line_parser(argc, argv).options(desc).positional(p).run(),
			vm);
		// Display help text when requested
		if(vm.count("help")){
			std::cout << desc << std::endl;
			exit(EXIT_SUCCESS);
		}
		po::notify(vm);
	} catch (std::exception &e) {
		std::cout << std::endl << e.what() << std::endl;
		std::cout << desc << std::endl;
		exit(EXIT_SUCCESS);
	}

	return vm;
}


int main (int argc, char **argv){
	po::variables_map options = process_program_options(argc, argv);

	bool sparse = options.count("sparse");
	string infile = options["input-file"].as<string>();

	if(options.count("output-file")){
		string outfile = options["output-file"].as<string>();
		ofstream out(outfile);
		process(infile, out, sparse);
		out.close();
	} else
		process(infile, cout, sparse);

	return 0;
}
