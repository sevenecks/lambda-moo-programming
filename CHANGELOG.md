# Changelog for LambdaMOO Programming Repository

## [1.1.9] - 2022.01.06 5:57PM
TOASTSTUNT PROGRAMMERS GUIDE
* Updated server options section to a nice table
* Added / updated options in server options section
* Added prettier table for variables that always have values
* Added prettier table for prepositions

## [1.1.8] - 2022.01.06 5:43PM
TOASTSTUNT PROGRAMMERS GUIDE
* Added a warning about anon objects being added to .owned_objects prop on create in ToastCore
* Updated section on protected properties to no longer call out this option as having a signficant performance cost
* Updated command line starting the server options to a nice table
* Added waif and last_move options to command line table
* Updated sqlite_execute and sqlite_query and sort as being threaded
* Updated reverse builtin to show it accepts strings
* Updated locations builtin with 2nd and 3rd arg + usage
* Removed process_id builtin as it was removed in Toast
* Incremened guide to 1.0.1

## [1.1.7] - 2022.01.05 8:20pm
* Added SindomeScheduler to Moo Code directory
* Updated Main Guide page to reference new code
* Added 1.0 header to ToastStunt Programmers Guide w/ Last Updated Date

## [1.1.6] - 2022.01.05 12:10pm

TOAST STUNT PROGRAMMERS MANUAL
Thanks ToastStunt Discord for all the feedback/updates/changes to the Toast Programmers Guide!
* Updated finding WAIFs example code to be the new code created by lisdude that finds anons as well
* Updated Working with Anonymous Objects section with new note on verb to find anon references in code
* fixed a number of typos/formatting issues throughout the guide 
* updated working with WAIFs section to add info on recursively referencing each other
* fixed formatting on \*anonymous\*
* fixed typo in set_thread_mode(0)
* changed note to warning on using anon objects as map keys
* updated typeof builtin to have ANON/WAIF/BOOL/MAP
* removed warning about BYTECODE_REDUCE_REF as it was not proven to be true
* changed wording in a few places to be more specific
* removed line: "Consider using a WAIF whenever you want to collect values together and give each value a meaningful name instead of just an index into a LIST." as it isn't neded now that we have MAPs
* Updated Objects in the MOO database section (updated wording, fixed ToastCore DB link)
* Removed LambdaMOO reference in Fundamental Object Attributes
* Updated properties on objects section to correctly say eleven props not eight
* Added recreate to functions that change parent and child attributes
* Resolved several TODOs
* Updated parse_json section to remove it not handling bools since we handle them properly now
* Updated fundamental object attributes

## [1.1.5] - 2022.01.04 2:46PM

TOAST STUNT PROGRAMMERS MANUAL
* This update brings in changes from Stunt Programmers Guide v10
* Updated Anonymous Object section with details from Stunt manual, and expanded on it with code examples.
* Added Working With Anonymous Objects section
* Fixed some typos
* Updated create() to add the 3rd (anon object) and 4th (args to pass to :initialize) arguments
* Added disclaimer about WAIF and Anon object creation and how it should be carefully monitored
* Added sample code for finding WAIFs in systems
* Updated Bitwise operators section w/ content from newest Stunt manual
* Added random_bytes() builtin
* Updated index() and rindex() with additional args and content from newest Stunt manual
* Added strtr() builtin
* Added decode_base64 builtin
* Added encode_base64 builtin
* Added salt builtin
* updated crypt builtin w/ Stunt info
* updated binary_hash & string_hash builtin info
* updated string_hmac and binary_hmac builtin info
* added process_id builtin
* updated server_log builtin
* updated server_verison builtin
* updated memory_usage builtin for ToastStunt
* added all_members builtin
* updated anonymous object type section
* moved working with WAIFs section
* added builtins for Toast/Stunt: all_members, ancestors, chr, clear_ancestor_cache, connection_info, connection_name_lookup, curl, descendants, exec, exp, explode, frandom, ftime, getenv, isa, locate_by_name, locations, mapdelete, maphaskey, mapkeys, mapvalues, memory_sage, next_recycled_object, occupants, owned_objects, panic, read_http, recreate, recycled_objects, reseed_random, respond_to, reverse, set_task_local, set_thread_mode, slice, sort, spellcheck, switch_player, task_local,
  thread_info, thread_pool, threads, usage, waif_stats, yin
* added sqlite section with: sqlite_close, sqlite_execute, sqlite_handles, sqlite_info, sqlite_interrupt, sqlite_last_insert_row_id, sqlite_limit, sqlite_open, sqlite_query
* Updated TOC

## [1.1.4] - 2022.01.03 10:01PM

TOAST STUNT PROGRAMMERS MANAUL
* Updated crypt() builtin
* Added argon2() builtin info
* Added argon2_verify() builtin info
* Added Threading section
* Added BitWise Operators section
* Updated $server_options.protect_`property` section to denote that it prevents non-wizard writes as well
* Added $server_options.max_list_concat
* Added $server_options.max_string_concat
* Added $server_options.max_contact_catchable
* Updated $server_options.max_stack_depth info
* Added note about how protect_verb and protect_prop function in relation to #0
* Added note on $server_options.protect_`prop` being disabled by default in options.h
* Updated TOC

## [1.1.3] - 2022.01.03 12:22PM

TOAST STUNT PROGRAMMERS MANUAL
* Added new section on Primitives
* Updated is_member() builtin
* added to $server_options: max_queued_output, dump_interval, proxy_rewrite, file_io_max_files, sqlite_max_handles, task_lag_threshold, finished_tasks_limit, no_name_lookup
* added note on `load_server_options()`
* added information on the + operator and list concatenation and addition
* updated references to `connection_option()` to point to `connection_options()`
* updated documentation for connection_options()
* Added information in tasks section about default_lag_threshold
* Added information on MAX_QUEUED_OUTPUT to various sections
* Updated TOC

## [1.1.2] - 2022.01.02 2:26PM

TOAST STUNT PROGRAMMERS MANUAL
* Integrated all of FileIO documentation into ToastStunt guide into Operations on Files section
* Added in new ToastStunt file_grep, file_count_lines, and file_handlers builtins
* Updated WAIF documentation
* Added in Perl Regular Expressions Section
* Updated ordering of TOC
* Added info on user friendly tracebacks
* Updated queue_info builtin
* Added last_move prop to list of props on objects
* Added Operations on Bools section
* Updated formatting on a bunch of builtins to make uniform
* Updated move() builtin
* Updated notify() builtin
* Updated queued_tasks() builtin
* Updated task_stack() builtin
* Updated section previously referring to $dump_interval to $server_options.dump_interval
* Add info about ToastStunt setting $maxint and $minint
* Added example @display of WAIF
* Added info on WAIFs & Map Style WAIF accessing
* 

## [1.1.1] - 2022.01.01 10:17pm

* Added beta version of ToastStunt Programmers Manual
* Updated README.md
* Updated ChangeLog
* Removed a few files that were WIP for the ToastStunt manual

## [1.1] - 2021.04.20 4:29pm

* Added new fork on documentation for ToastStunt

## [1.0.2A] - 2020.07.20 6:29pm

* Merged in a PR from stevenjowens that fixed formatting in their guide

## [1.0.2] - 2019.06.02 6:14PM

* Fixed broken link to CHANGELOG.md

## [1.0.1] - 2019.06.02 6:09PM

* Fixed typo

## [1.0] - 2019.06.02 1:25PM

* Finished editing the Updated lambdamoo programming manual. All HTML converted, all errors resolved. Releasing as initial major version.
* Rendered new version of updated moo programming manual in markdown
* Updated README to include example of how to contribute
* Updated TODOs
* Updated years in license

## [0.14] - 2019.01.08 10:31PM

* Added code scanner to useful moo code section
* Updated readme
* Other updates happened prior to this, I forgot to update the change log.

## [0.13] - 2017.07.19 6:24PM
* Updated the CSS on the HTML5 version of the updated moo programming guide
* Added link to this repo to the html5 version
* Updated the remote Sindome MOO-manual.html reference to the new version

## [0.12] - 2018.07.10 2:35PM
* Added server setup section to readme
* Added automated local environment setup (Stunt & LambdaCore) section to readme

## [0.11] - 2018.06.21 9:54AM
* Fixed local instruction code

## [0.10] - 2018.06.21 9:46AM
* Removed goblin extension at authors request

## [0.9] - 2018.06.20 10:53PM
* Fixed markdown links
* Added code to readme
* Added patches to readme
* Added webclient section to readme
* Added public webclient section to readme

## [0.8] - 2018.06.20 10:38PM
* Added Patche for goblin extension (probably only works on gammamoo)
* Added patch for fileio-1.5p3 (Todd's version)
* Added 'Code' directory and a code example for getting local editing (in a pop up) working form Dome-Client

## [0.7] - 2018.03.16 3:12PM
* Added genesismud tutorial by Puff
* Added emacs mud.el tutorial by Puff
* Updated README

## [0.6] - 2018.03.07 2:49PM
* Updated README with markdown conversion instructions
* Updated Steven Owen guide with new version (md and html)

## [0.5] - 2018.01.31 4:30PM

### Updated
* Naming of various files
* Updated License year

### Added
* Markdown versions of all files
* HTML for yibs was run through an HTML tidier
* <object> removed from yibs tutorial as it was breaking markdown conversion
* Updates to README (index, better information on repo)
* Repository renamed to lambda-moo-programming
* Added directory structure with tutorials and tutorials/src
* Organized files

## [0.4] - 2018.01.26 5:28PM

### Added
* Finished adding all functions to moo.html

## [0.3] - 2018.01.25 1:56PM

### Added
* Added dark-sleep tutorial
* Added hacking lambda moo server tutorial
* Added LambdaMOO Background doc
* Added Way Easy MOO Programming Guide
* Added Winding Duck tutorial
* Added Yib's pet rock tutorial
* Added Zompost moo help doc
* Updated README

## [0.2] - 2018.01.22 5:00PM

### Added
* lisdude web resource
* changelog.md
* Changed LambdaMOO sourcecode github reference to SevenEcks (includes patches and stuff)
* Added MOOTalk Mailing list to resources
* Replaced two spaces after periods with one space after periods (736 occurances)
* Dome Client resources link
* New Note on MOO Clients
* Added new notes on extensions, and extensions throwing their own errors

## [0.1] - 2016.09.13 5:35PM

### Added
* Initial Release
* HTML5 Conversion
* Table of Contents
* Additional Comments
* Clarifications
* Updated Resources
* Code Sdamples
