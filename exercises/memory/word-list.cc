// Compile: c++ -g -W -Wall -ansi -pedantic -o word-list word-list.cc -ldl -lpthread
#include <string>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <set>
#include <regex.h>
#include <dlfcn.h>
#include <unistd.h>

union conversion { void *ptr; void (*dump)(const char *pathname); } drop_profile;

void
profile_snapshot(const char *label)
{
  if (drop_profile.dump)
  {
    char filename[128];
    snprintf(filename, sizeof(filename),
	     "|gzip -9c>igprof.word-list.%d.%ld.%.20s.gz",
             (int) getpid(), (long) time(0), label);
    drop_profile.dump(filename);
  }
}

/// Read words from file @a filename. Reads the file and extracts all
/// white-space separated tokens. Returns the result as a string array.
/// All words in the file are returned; duplicates are not removed.
std::vector<std::string>
read_words(std::string filename)
{
  std::ifstream file(filename.c_str());
  std::vector<std::string> result;
  std::copy(std::istream_iterator<std::string>(file),
	    std::istream_iterator<std::string>(),
	    std::back_inserter(result));
  profile_snapshot("read_words.post");
  return result;
}

/// Filter out strings matching regular expression string @a pattern
/// from the list of words @a word_list.  Returns the remaining words,
/// in the same order as the original list.  Throws an exception if
/// the regular expression is invalid (std::runtime_error).
std::vector<std::string>
filter_out_words(std::string pattern, std::vector<std::string> word_list)
{
  profile_snapshot("filter_out_words.pre");

  // Build a regexp and barf if it's not ok.
  int err;
  regex_t rx;
  if ((err = regcomp(&rx, pattern.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB)))
  {
    char *errmsg = 0;
    size_t errlen = 0;
    size_t needed;
    while ((needed = regerror(err, &rx, errmsg, errlen)) > errlen)
    {
      delete errmsg;
      errlen = needed+1;
      errmsg = new char[errlen];
    }
    throw std::runtime_error(pattern + ": invalid regular expression: " + errmsg);
  }

  // Scan the list of words, and filter out those that match the regexp.
  std::vector<std::string>::iterator i = word_list.begin();
  while (i != word_list.end())
  {
    if (regexec(&rx, i->c_str(), 0, 0, 0) == 0)
      i = word_list.erase(i);
    else
      ++i;
  }

  // Return the remaining words.
  profile_snapshot("filter_out_words.post");
  return word_list;
}

/// Return the list of words in alphabetically sorted order,
/// with duplicate words removed.
std::vector<std::string>
sort_words_uniq(std::vector<std::string> word_list)
{
  profile_snapshot("sort_words_uniq.pre");
  std::set<std::string> ordered(word_list.begin(), word_list.end());
  std::vector<std::string> result(ordered.begin(), ordered.end());
  profile_snapshot("sort_words_uniq.post");
  return result;
}

/// Main program.
int main (int argc, char **argv)
{
  if (argc < 3)
  {
    std::cout << "usage: " << argv[0] << " snap pattern [file...]\n";
    return 1;
  }

  if (*argv[1] == 'Y')
    drop_profile.ptr = dlsym(0, "igprof_dump_now");

  for (int i = 3; i < argc; ++i)
  {
    try
    {
      std::vector<std::string> words
        = sort_words_uniq(filter_out_words(argv[2], read_words(argv[i])));
      std::cout << argv[i] << ": " << words.size()
		<< " words do not match regex '" << argv[2]
		<< "', the middle words are:\n";
      int middle = words.size() / 2;
      for (int w = middle - 5, e = middle + 5; w < e; ++w)
	std::cout << "   '" << words[w] << "'\n";
    }
    catch (std::runtime_error &err)
    {
      std::cout << argv[i] << ": error: " << err.what() << std::endl;
    }
  }

  profile_snapshot("main.post");
  return 0;
}
