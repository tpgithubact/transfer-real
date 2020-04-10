

#include "ndnpeek.hpp"
#include "core/version.hpp"

#include <ndn-cxx/util/io.hpp>

namespace ndn {
namespace peek {

namespace po = boost::program_options;

static void
usage(std::ostream& os, const po::options_description& options)
{
  os << "Usage: ndnpeek [options] ndn:/name\n"
        "\n"
        "Fetch one data item matching the name prefix and write it to standard output.\n"
        "\n"
     << options;
}

static int
main(int argc, char* argv[])
{
  PeekOptions options;

  po::options_description genericOptDesc("Generic options");
  genericOptDesc.add_options()
    ("help,h", "print help and exit")
    ("payload,p", po::bool_switch(&options.wantPayloadOnly),
        "print payload only, instead of full packet")
    ("timeout,w", po::value<int>(),
        "set timeout (in milliseconds)")
    ("verbose,v", po::bool_switch(&options.isVerbose),
        "turn on verbose output")
    ("version,V", "print version and exit")
  ;

  po::options_description interestOptDesc("Interest construction");
  interestOptDesc.add_options()
    ("prefix,P", po::bool_switch(&options.canBePrefix),
        "set CanBePrefix")
    ("fresh,f", po::bool_switch(&options.mustBeFresh),
        "set MustBeFresh")
    ("link-file", po::value<std::string>(),
        "set ForwardingHint from a file")
    ("lifetime,l", po::value<int>(),
        "set InterestLifetime (in milliseconds)")
  ;

  po::options_description visibleOptDesc;
  visibleOptDesc.add(genericOptDesc).add(interestOptDesc);

  po::options_description hiddenOptDesc;
  hiddenOptDesc.add_options()
    ("name", po::value<std::string>(), "Interest name");

  po::options_description optDesc;
  optDesc.add(visibleOptDesc).add(hiddenOptDesc);

  po::positional_options_description optPos;
  optPos.add("name", -1);

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).options(optDesc).positional(optPos).run(), vm);
    po::notify(vm);
  }
  catch (const po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 2;
  }

  if (vm.count("help") > 0) {
    usage(std::cout, visibleOptDesc);
    return 0;
  }

  if (vm.count("version") > 0) {
    std::cout << "ndnpeek " << tools::VERSION << std::endl;
    return 0;
  }

  if (vm.count("name") > 0) {
    options.name = vm["name"].as<std::string>();
  }
  else {
    std::cerr << "ERROR: Interest name is missing" << std::endl;
    usage(std::cerr, visibleOptDesc);
    return 2;
  }

  if (vm.count("lifetime") > 0) {
    if (vm["lifetime"].as<int>() >= 0) {
      options.interestLifetime = time::milliseconds(vm["lifetime"].as<int>());
    }
    else {
      std::cerr << "ERROR: InterestLifetime must be a non-negative integer" << std::endl;
      usage(std::cerr, visibleOptDesc);
      return 2;
    }
  }

  if (vm.count("timeout") > 0) {
    if (vm["timeout"].as<int>() > 0) {
      options.timeout = time::milliseconds(vm["timeout"].as<int>());
    }
    else {
      std::cerr << "ERROR: Timeout must be a positive integer" << std::endl;
      usage(std::cerr, visibleOptDesc);
      return 2;
    }
  }

  if (vm.count("link-file") > 0) {
    options.link = io::load<Link>(vm["link-file"].as<std::string>());
    if (options.link == nullptr) {
      std::cerr << "ERROR: Cannot read Link object from the specified file" << std::endl;
      usage(std::cerr, visibleOptDesc);
      return 2;
    }
  }

  Face face;
  NdnPeek program(face, options);

  try {
    program.start();
    face.processEvents(program.getTimeout());
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }

  ResultCode result = program.getResultCode();
  if (result == ResultCode::TIMEOUT && options.isVerbose) {
    std::cerr << "TIMEOUT" << std::endl;
  }
  return static_cast<int>(result);
}

} // namespace peek
} // namespace ndn

int
main(int argc, char** argv)
{
  return ndn::peek::main(argc, argv);
}
