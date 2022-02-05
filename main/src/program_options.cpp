#include <iostream>

#include <program_options.hpp>

namespace {

namespace b_opt = boost::program_options;

cs7gv5::program_options_t opts;

} // namespace

const cs7gv5::program_options_t &cs7gv5::parse_opts(int argc, char *argv[]) {
  b_opt::options_description desc("all options");
  desc.add_options()("help,h", "");
  desc.add_options()("vert,v", b_opt::value<std::string>());
  desc.add_options()("frag,f", b_opt::value<std::string>());
  desc.add_options()("obj,o", b_opt::value<std::string>());

  b_opt::variables_map vm;

  b_opt::store(b_opt::parse_command_line(argc, argv, desc), vm);

  b_opt::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    exit(0);
  }

  if (!vm.count("vert") || !vm.count("frag") || !vm.count("obj")) {
    std::cout << desc << std::endl;
    exit(1);
  }

  opts.vert = vm["vert"].as<std::string>();
  opts.frag = vm["frag"].as<std::string>();
  opts.model = vm["obj"].as<std::string>();

  return opts;
}