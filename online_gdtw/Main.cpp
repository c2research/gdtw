#include <chrono>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <boost/tokenizer.hpp>

#if defined(__linux__) || defined(__APPLE__)

#include <readline/readline.h>
#include <readline/history.h>

#elif defined(_WIN32) || defined(_WIN64)

#include <editline/readline.h>

#endif

#include "Command.hpp"
#include "Exception.hpp"
#include "GDTWOnline.hpp"

#include "config.hpp"

GDTWOnline gGDTWOnline;
bool gTimerEnabled = true;

bool tooFewArgs(const std::vector<std::string>& args, int limit)
{
  if (args.size() < limit) {
    std::cout << "Error! Too few arguments" << std::endl;
    return true;
  }
  return false;
}

bool tooManyArgs(const std::vector<std::string>& args, int limit)
{
  if (args.size() > limit) {
    std::cout << "Error! Too many arguments" << std::endl;
    return true;
  }
  return false;
}

/**************************************************************************
 * HOW TO CREATE A NEW COMMAND
 *
 * Step 1: Define a new command using the MAKE_COMMAND macro
 *
 * MAKE_COMMAND(<command_name>, <code>, <help_summary>, <help>)
 *  <command_name> - name of the command
 *  <code>         - the code being executed when this command is called.
 *                   Arguments are put in a vector of string call 'args'.
 *                   This code should return true if the command is carried
 *                   out successfully and false otherwise.
 *  <help_summary> - a short text line describing the command. This is
 *                   showed next to the command name when 'help' is called
 *  <help>         - a long text describe the command in details. This is
 *                   showed when 'help <command_name>' is called
 *
 * This macro creates a class Cmd<command_name> and its object in a variable
 * called cmd<command_name>. This variable is used in step 2.
 **************************************************************************/

MAKE_COMMAND(LoadDataset,
  {
    if (tooFewArgs(args, 2) || tooManyArgs(args, 5))
    {
      return false;
    }

    std::string filePath = args[1];
    int maxNumRow = args.size() > 2 ? std::stoi(args[2]) : 0;
    int startCol  = args.size() > 3 ? std::stoi(args[3]) : 0;
    std::string separators = args.size() > 4 ? args[4] : " ";

    dataset_info_t info;
    try
    {
      info = gGDTWOnline.loadDataset(filePath, maxNumRow, startCol, separators);
    }
    catch (GenexException& e)
    {
      std::cout << "Error! " << e.what() << std::endl;
      return false;
    }

    std::cout << "Dataset loaded                    " << std::endl
              << "  Name:        " << info.name       << std::endl
              << "  ID:          " << info.id         << std::endl
              << "  Item count:  " << info.itemCount  << std::endl
              << "  Item length: " << info.itemLength << std::endl;

    return true;
  },

  "Load a dataset to the memory",

  "Dataset are text files with table-like format, such as comma-separated  \n"
  "values files.                                                           \n"
  "                                                                        \n"
  "Usage: load <filePath> [<maxNumRow> <startCol> <separators>]            \n"
  "  filePath  - Path to a text file containing the dataset                \n"
  "  maxNumRow - Maximum number of rows will be read from the file. If this\n"
  "              number is non-positive or the number of actual line is    \n"
  "              smaller than this number, all lines are read. (default: 0)\n"
  "  startCol  - Omit all columns before this column. (default: 0)         \n"
  "  seprators - A list of characters used to separate values in the file  \n"
  "              (default: <space>)                                          "
  )

MAKE_COMMAND(UnloadDataset,
  {
    if (tooFewArgs(args, 2) || tooManyArgs(args, 5))
    {
      return false;
    }

    int index = std::stoi(args[1]);

    try
    {
      gGDTWOnline.unloadDataset(index);
    }
    catch (GenexException& e)
    {
      std::cout << "Error! " << e.what() << std::endl;
      return false;
    }

    std::cout << "Dataset " << index << " is unloaded" << std::endl;
    return true;
  },

  "Unload a dataset from the memory",

  "Usage: unload <dataset_index>                               \n"
  "  dataset_index  - Index of the dataset being unloaded. Use \n"
  "                   'list dataset' to retrieve the list of   \n"
  "                   loaded datasets.                           "
  )

MAKE_COMMAND(List,
  {
    if (tooFewArgs(args, 2) || tooManyArgs(args, 2))
    {
      return false;
    }

    if (args[1] == "dataset")
    {
      std::vector<dataset_info_t> infos = gGDTWOnline.getAllDatasetInfo();
      std::cout << "There are " << infos.size() << " loaded datasets" << std::endl;
      for (const auto& i : infos)
      {
        std::cout << "  " << std::setw(4) << i.id << "\t" << i.name << std::endl;
      }
    }
    else if (args[1] == "metric") {
      std::vector<distance_info_t> infos = gGDTWOnline.getAllDistanceInfo();
      for (const auto& i : infos)
      {
        std::cout << " " << std::setw(10) << i.name << "\t" << i.description << std::endl;
      }
    }
    else
    {
      std::cout << "Error! Unknown object: " << args[1] << std::endl;
      return false;
    }
    return true;
  },

  "List loaded dataset or available metric",

  "Usage: list dataset|metric"
  )

MAKE_COMMAND(Distance,
  {
    if (tooFewArgs(args, 10) || tooManyArgs(args, 10))
    {
      return false;
    }

    int ds1 = std::stoi(args[1]);
    int ts1 = std::stoi(args[2]);
    int start1 = std::stoi(args[3]);
    int length1 = std::stoi(args[4]);
    int ds2 = std::stoi(args[5]);
    int ts2 = std::stoi(args[6]);
    int start2 = std::stoi(args[7]);
    int length2 = std::stoi(args[8]);

    try
    {
      data_t result = gGDTWOnline.getDistanceBetween(ds1, ts1, start1, length1,
                                                     ds2, ts2, start2, length2, args[9]);

      std::cout << "Using distance " << args[9] << std::endl
                << "Time series 1 [ds: " << ds1 << " id: " << ts1
                << " start: " << start1 << " length: " << length1 << "]" << std::endl
                << "Time series 2 [ds: " << ds2 << " id: " << ts2
                << " start: " << start2 << " length: " << length2 << "]" << std::endl;

      std::cout << "Result: " << std::setprecision(4) << std::fixed << result << std::endl;

    }
    catch (GenexException& e)
    {
      std::cout << "Error! " << e.what() << std::endl;
      return false;
    }

    return true;
  },

  "Calculate the distance between two time series",

  "The two time series may come from the same dataset or two different datasets.            \n"
  "Usage: distance <ds1> <ts1> <start1> <length1> <ds2> <ts2> <start2> <length2> <distance> \n"
  "  ds1      - Index of the dataset containing the first time series.                      \n"
  "  ts1      - Index of the first time series in the dataset.                              \n"
  "  start1   - Starting position in the first time series.                                 \n"
  "  length1  - Length of the first time series.                                            \n"
  "  ds2      - Index of the dataset containing the second time series.                     \n"
  "  ts2      - Index of the second time series in the dataset.                             \n"
  "  start2   - Starting position in the second time series.                                \n"
  "  length2  - Length of the second time series.                                           \n"
  "  distance - Name of distance to use.                                                    \n"
  )

MAKE_COMMAND(Timer,
  {
    if (tooFewArgs(args, 1) || tooManyArgs(args, 2))
    {
      return false;
    }
    if (args.size() == 1)
    {
      std::cout << "Timer is " << (gTimerEnabled ? "ON" : "OFF") << std::endl;
    }
    else
    {
      if (args[1] == "on")
      {
        gTimerEnabled = true;
        std::cout << "Timer is ON" << std::endl;
      }
      else if (args[1] == "off")
      {
        gTimerEnabled = false;
        std::cout << "Timer is OFF" << std::endl;
      }
      else
      {
        std::cout << "Error! Argument for timer must be 'on' or 'off'" << std::endl;
        return false;
      }
    }
    return true;
  },

  "Turn timer on or off",

  "When timer is turned on, each command is timed and reported at  \n"
  "the end of its execution. If this command is called without     \n"
  "an additional argument, the current state of timer is printed.  \n"
  "                                                                \n"
  "Usage: timer [on|off]                                             "
)

MAKE_COMMAND(Best,
  {
    if (tooFewArgs(args, 4) || tooManyArgs(args, 7))
    {
      return false;
    }

    std::string dist = "euclidean_dtw";

    if (args.size() == 7)
    {
      dist = args[6];
    }

    int ds1 = std::stoi(args[1]);
    int ts1 = std::stoi(args[2]);
    int start1 = std::stoi(args[3]);
    int length1 = std::stoi(args[4]);
    int ds2 = std::stoi(args[5]);

    try
    {
      candidate_time_series_t result =
          gGDTWOnline.getBestMatchRaw(ds1, ts1, start1,
                                    length1, ds2, dist);

      data_t dist = result.dist;
      TimeSeries data = result.data;

      int ts2 = data.getIndex();
      int start2 = data.getStart();
      int length2 = data.getLength();

      std::cout << "Using distance " << dist << std::endl
                << "Time series 1 [ds: " << ds1 << " id: " << ts1
                << " start: " << start1 << " length: " << length1 << "]" << std::endl
                << "Best Interval: "
                << "[ds: " << ds2 << " id: " << ts2
                << " start: " << start2 << " length: " << length2 << "]" << std::endl;

      std::cout << "Result: " << std::setprecision(4) << std::fixed << dist << std::endl;
    }
    catch (GenexException &e)
    {
      std::cout << "Error! " << e.what() << std::endl;
      return false;
    }
    return true;
  },

  "Raw compute, find the best (closest) interval in a dataset",

  "Include the index of the dataset of the target (query) time series\n"
  "and the search dataset, the dataset you wish to search through.   \n"
  "defaults to euclidean distance.                                   \n"
  "                                                                  \n"
  "Usage: best <dsT> <idxT> <start> <length> <dsS> [dist]            \n"
  " dsT    - dataset index of target time series                     \n"
  " idxT   - index of time series in dsT                             \n"
  " start  - start index of interval in time series                  \n"
  " length - length of interval                                      \n"
  " dist   - optional, the distance function you wish to use         \n"
  "                                                                  \n"
  " NOTE: If using the UCR dataset, when loading data be sure        \n"
  "       to skip the first column.                                  \n"
)

MAKE_COMMAND(SetDTWPathFile,
  {
    if (tooFewArgs(args, 2) || tooManyArgs(args, 2))
    {
      return false;
    }
    try
    {
      gGDTWOnline.setPrintDTWPathToFile(args[1]);
    }
    catch (GenexException &e)
    {
      std::cout << "Error! " << e.what() << std::endl;
      return false;
    }
    std::cout << "File " << args[1] << " created." << std::endl;
    return true;
  },

  "Set file where the DTW path will be appended to",

  "Usage: setDTWFile <path>     \n"
  " <path>    - path to a file  \n"
)

/**************************************************************************
 * Step 2: Add the Command object into the commands map
 *
 * The key is a string which is also the string that is used to match with
 * user input. The value is the cmd<command_name> variable created in step 1.
 **************************************************************************/

std::map<std::string, Command *>
    commands = {
        {"load", &cmdLoadDataset}, 
        {"unload", &cmdUnloadDataset}, 
        {"list", &cmdList}, 
        {"timer", &cmdTimer}, 
        {"distance", &cmdDistance}, 
        {"best", &cmdBest},
        {"setDTWFile", &cmdSetDTWPathFile}
};

/**************************************************************************/
/**************************************************************************/

#define COUT_HELP_ALIGNMENT 15

    const std::string HELP_SUMMARY = "Retrieve a list of commands or get help for a command";
    const std::string HELP_HELP = "Usage: help [<command_name>]                                \n"
                                  "  command_name - Name of command to retrieve help about. If \n"
                                  "                 not specified, a list of available commands\n"
                                  "                 is shown instead.                            ";
    const std::string EXIT_SUMMARY = "Terminate the program";
    const std::string EXIT_HELP = "Usage: Can use either 'exit' or 'quit'";

    void showHelp(const std::string &command_name)
    {
      if (command_name == "help")
      {
        std::cout << HELP_SUMMARY << std::endl
                  << HELP_HELP << std::endl;
      }
      else if (command_name == "exit" || command_name == "quit")
      {
        std::cout << EXIT_SUMMARY << std::endl
                  << EXIT_HELP << std::endl;
      }
      else if (commands.find(command_name) != commands.end())
      {
        Command *cmd = commands[command_name];
        std::cout << cmd->getSummary() << std::endl
                  << cmd->getHelp() << std::endl;
      }
      else
      {
        std::cout << "Error! Cannot find help for command: " << command_name << std::endl;
      }
}

void showAllHelps()
{
  std::cout << "Use 'help <command>' to see help for a command" << std::endl << std::endl;

  std::cout << std::setw(COUT_HELP_ALIGNMENT);
  std::cout << "help" << HELP_SUMMARY << std::endl;

  std::cout << std::setw(COUT_HELP_ALIGNMENT);
  std::cout << "exit|quit " << EXIT_SUMMARY << std::endl;

  std::cout << std::endl;
  for (const auto& cmd : commands)
  {
    std::cout << std::setw(COUT_HELP_ALIGNMENT);
    std::cout << cmd.first << cmd.second->getSummary() << std::endl;
  }
}

bool processLine(const std::string& line)
{
  boost::char_separator<char> sep(" ");
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  tokenizer tokens(line, sep);
  std::vector<std::string> args (tokens.begin(), tokens.end());

  if (args.size() == 0)
  {
    return false;
  }

  if (args[0] == "quit" || args[0] == "exit")
  {
    return true;
  }
  else if (args[0] == "help")
  {
    if (args.size() == 1)
    {
      showAllHelps();
    }
    else if (args.size() == 2)
    {
      showHelp(args[1]);
    }
    else
    {
      std::cout << "Error! Too many arguments for 'help'" << std::endl;
    }
  }
  else
  {
    if (commands.find(args[0]) == commands.end())
    {
      std::cout << "Error! Cannot find command: " << args[0] << std::endl;
    }
    else {
      std::chrono::time_point<std::chrono::system_clock> start, end;
      start = std::chrono::system_clock::now();

      bool success = commands[args[0]]->doCommand(args);

      end = std::chrono::system_clock::now();
      if (gTimerEnabled && success)
      {
        std::chrono::duration<float> elapsed_seconds = end - start;
        std::cout << std::endl << "Command executed in ";
        std::cout << std::setprecision(4) << elapsed_seconds.count() << "s" << std::endl;
      }
    }
  }

  return false;
}

bool wantToQuitByEOF()
{
  std::cout << std::endl;
  char* raw_line = nullptr;
  do
  {
    delete raw_line;
    raw_line = readline("Do you really want to exit ([y]/n)? ");
    if (raw_line == nullptr || strcmp(raw_line, "y") == 0 || strcmp(raw_line, "") == 0)
    {
      return true;
    }
  }
  while (strcmp(raw_line, "n") != 0);
  return false;
}

int main (int argc, char *argv[])
{
  bool quit = false;
  char* raw_line;

  // Align left
  std::cout << std::left;
  std::cout << "Welcome to GDTW Online!\n"
               "Use 'help' to see the list of available commands." << std::endl;

  while (true)
  {
    quit = false;
    raw_line = readline(">> ");

    if (raw_line == nullptr) // Ctrl-D is hit or EOF
    {
      quit = wantToQuitByEOF();
    }
    else
    {
      if (strlen(raw_line) > 0)
      {
        add_history(raw_line);
      }
      std::string line = std::string(raw_line);
      delete raw_line;
      try
      {
        quit = processLine(line);
      }
      catch (std::logic_error& e)
      {
        std::cout << "Error! Cannot convert some value to numeric" << std::endl;
      }
      catch (...)
      {
        std::cout << "Error! Unknown but most likely memory problem. Exitting..." << std::endl;
        quit = true;
      }
      std::cout << std::endl;
    }

    if (quit)
    {
      break;
    }
  }
  return 0;
}
