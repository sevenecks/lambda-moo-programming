# Changelog for LambdaMOO Programming Repository
## [1.1.00] - 2023.03.06.23 1:29PM
* Added in connection_info() update
* Updated to 1.1 as ToastStunt 2.70 released on 03.05.23

## [1.0.11] - 2022.12.15 3:43PM
* Added Anatomy of ToastStunt Database link to the Resources section

## [1.0.10] - 2022.11.12 11:39AM
* Updated the connection_name() function definition to include missing information on changes to this in ToastStunt, and new method args available.

## [1.0.9] - 2022.09.01 11:53AM
* Updated the WAIF finding code to support better finding / searching in maps
* Updated curl() documentation and added CURL_TIMEOUT to Server Compilation Options 
* Updated generate_json and decode_json documentation to specifiy WAIF and ANON are not supported

## [1.0.8] - 2022.03.26 1:27PM
* Updated delete_verb, verb_info, set_verb_info, verb_args, set_verb_args, disasemble, and a few other verbs to better reference the ability to provide an INT index into verbs() instead of a str name.
* Updated `suspend()` function definition to call out sub-second suspend

## [1.0.7] - 2022.01.14 3:52PM
* Incremented guide to 1.0.7
* removed 'weak references' bullet as lisdude's suggestion as it was only adding confusion
* updated file abstration WAIF section to not mention it being in ToastCore as it isn't
* fixed some typos/case issues in the text
* removed a line from addtional details on WAIFs that had already been gone over earlier in the text
* removed extra 'map' in general operations applicable to all values
* fixed formatting on some of the builtins where they weren't on their own line
* Changed reference of Stunt to ToastStunt
* removed todo around argon since we got that infos
* Updated example on how to corify an object and called out $sysobj as typically being #0
* Fixed formatting on table for 'The values of some of these variables always start out the same'
* Fixed formatting of Server Configuration section

## [1.0.6] - 2022.01.10 12:21PM
* Fixed ToastCore.db URL
* Incremented guide to 1.0.6

## [1.0.5] - 2022.01.08 9:19PM
* Added Newbie guide to compiling ToastStunt to resources section
* Cleaned up some bad formatting due to < and >
* changed all smart quotes to `
* fixed up various formatting issues
* updated server_log builtin to specify INT as type for level

## [1.0.4] - 2022.01.08 2:29PM
* Added Server Signals section
* Updated info around `do_login_command()` to mention not suspending and referencing switch_player()
* Added finished_tasks() to server builtins and a nice pretty table
* Added Telnet IAC info in section on $do_out_of_band_command()
* Added opinion on anonymous objects being less good than waifs to the anon object section
* Added opinion on multiple inheritance and how you should avoid it.
* Added several links to the resources section and removed ones no longer needed
* Updated foreword
* Updated table of contents
* Incremented verison to 1.0.4
* New CHANGELOG just for the guide
* Added CONTRIBUTORS.md
* New guide folder

## [1.0.3] - 2022.01.07 12:22PM
Thanks to lisdude for all these suggestions
* Added explanatory text around last_move to Properties on Objects section
* Added permission bit table
* Added note on #0 / corification after first reference to a $ verb/prop
* added num, true, false, to section on variables that always have values
* fixed some typos 
* added a fancy table to bitwise operators
* added fancy table for property permissions
* removed fine point about toastcore not supporting @set/@Prop on bools as it does now! woo!
* updated inline try catch example from ANY to E_VERBNF
* Updated warning to not reference sindome specific $cu:sin and to reference $command_utils:suspend_if_needed, also mentioned yin()
* updated default fg and bg ticks -- removed todo around this
* updated the callout about default_lag_threshold with better content provided by lisdude
* added note about FG_SECONDS and BG_SECONDS not corresponding to real life seconds
* Removed warning about toastcore's :initialize verb and anon objects as it is no longer applicable
* Moved and updated warning about ToastStunt waif vs lambdamoo waif and added migration info
* incremented guide to 1.0.3

## [1.0.2] - 2022.01.06 9:49PM
* Updated server options section to a nice table
* Added / updated options in server options section
* Added prettier table for variables that always have values
* Added prettier table for prepositions
* Removed warning that slice() is threaded since it isn't anymore
* Updated the INT/OBJ/STR/etc table to include the value of these constants
* Updated explode builtin with more info / third arg
* Updated sqlite_execute to include regex info
* Updated exec with example for setting env variables

## [1.0.1] - 2022.01.06 5:43PM
* Added a warning about anon objects being added to .owned_objects prop on create in ToastCore
* Updated section on protected properties to no longer call out this option as having a signficant performance cost
* Updated command line starting the server options to a nice table
* Added waif and last_move options to command line table
* Updated sqlite_execute and sqlite_query and sort as being threaded
* Updated reverse builtin to show it accepts strings
* Updated locations builtin with 2nd and 3rd arg + usage
* Removed process_id builtin as it was removed in Toast
* Incremened guide to 1.0.1

## [1.0.0] - 2022.01.05 8:20pm
* Updated Main Guide page to reference new code
* Added 1.0 header to ToastStunt Programmers Guide w/ Last Updated Date

## [0.0.j] - 2022.01.05 12:10pm
* Thanks ToastStunt Discord for all the feedback/updates/changes to the Toast Programmers Guide!
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

## [0.0.5] - 2022.01.04 2:46PM
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

## [0.0.4] - 2022.01.03 10:01PM
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

## [0.0.3] - 2022.01.03 12:22PM
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

## [0.0.2] - 2022.01.02 2:26PM
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

## [0.0.1] - 2022.01.01 10:17pm
* Added beta version of ToastStunt Programmers Manual

## [0.0] - 2021.04.20 4:29pm
* Added new fork on documentation for ToastStunt
