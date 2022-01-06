// This provides a $scheduler that will run and manage scheduled tasks for your MOO
// This is the $scheduler that Sindome uses, and it has been heavily modified over the years to support a variety of use cases.
// This is designed to work on LambdaMOO and ToastCore, either one will suffice.
// However, that means that this is using classic LambdaMOO utils like $list_utils:slice instead of the ToastStunt builtin slice
// Update as you want, but don't use threaded stuff as you'll get multiple scheduler forks going at once which will not be great.

// create the object

@create #78 named MOO task Scheduler:MOO,task,Scheduler,MOO task Scheduler

// replace #112 with newly created object # everywhere in below file

// execute all the statements below, and then eval ;$scheduler:jesus() to get the scheduler running
// then create a test verb on yourself named 'tester' and have it print a message to you.
// the you can test the scheduler with ;$scheduler:schedule_for(10, me, "tester")
// you'll see the message when the scheduler runs
// $scheduler:schedule_every(10, me, "tester") will make the verb run every 10 seconds or so
// there are a number of other verbs for scheduling tasks
// remove a scheduled verb with $scheduler:remove_scheduled(me, "tester");
// by default allow_duplicates is off, meaning the same obj/verb can only be scheduled once
// @scheduled is a nice way to see what is scheduled especially if your scheduler gets big!
// @scheduled for #2
// @scheduled for soon
// @scheduled by #2
// help $scheduler for more info

@corify #112 as $scheduler

@prop #112.run_task 0

@prop #112."scheduled_tasks" {} r

;;#112.("scheduled_tasks") = {}

@prop #112."processing" 0 r

@prop #112."kill_tasks" {} r

@prop #112."killed_tasks" {} r

;;#112.("killed_tasks") = {}

@prop #112."jesus_running" 0 r

@prop #112."incoming_tasks" {} r

;;#112.("help_msg") = {"VIEWING ALL SCHEDULED TASKS", "", "To view all scheduled tasks you can 'look $scheduler' OR type '@scheduled' with no arguments.", "", "VIEWING A PORTION OF SCHEDULED TASKS", "", "@scheduled allows you to slice and dice scheduled tasks as you see fit. Here are the options at your disposal:", "", "@scheduled repeat                             => display only scheduled tasks that are repeating", "", "@scheduled norepeat                           => display only scheduled tasks that do no repeat", "", "@scheduled soon                               => display tasks that are scheduled to run in the next 5 minutes", "", "@scheduled later                              => display tasks that are scheduled to run later than 5 minutes", "", "@scheduled interval                           => display scheduled tasks that are set to run at a varied interval IE: [200, 300]", "", "@scheduled verb                               => display scheduled tasks who's reschedule time is determined by a verb", "", "@scheduled timestring                         => display scheduled tasks that are rescheduled based on a time string IE: 15:00", "", "@scheduled seconds                            => display scheduled tasks that are rescheduled based on a set second based interval IE: 300", "", "@scheduled duplicates                         => display duplicate obj&verb combinations in the $scheduler", "", "@scheduled by <obj or name>                   => display verbs scheduled by a specific person", "", "@scheduled for <obj|corified ref or verbname> => display verbs scheduled for a specific object or verb", "", "@scheduled without <verb>                     => display verbs scheduled, excluding <verb>"}

;;#112.("aliases") = {"MOO", "task", "Scheduler", "MOO task Scheduler"}

;;#112.("description") = "The Almighty Scheduler"

@verb #112:"run_scheduled" this none this
@verb #112:"schedule_for" this none this
@verb #112:"schedule_at" this none this
@verb #112:"start halt" this none this
@verb #112:"schedule_every" this none this
@verb #112:"look_self" this none this
@verb #112:"remove_scheduled" this none this
@verb #112:"jesus" this none this rxd
@verb #112:"is_scheduled" this none this rxd 
@verb #112:"match" this none this rxd
@verb #112:"when_scheduled" this none this rxd #98
@verb #112:"next_runtime" this none this rxd #98
@verb #112:"check_repeat_args" this none this rxd #98
@verb #112:"describe_repeat" this none this rxd
@verb #112:"display_schedule" this none this rxd
@verb $code_utils:core_references tnt rxd
@verb me:@scheduled any any any rxd

@args #112:"run_scheduled" this none this
@program #112:run_scheduled
"run_scheduled( [INT kill]) => if kill is provided and true then kill the scheduler task, otherwise start it";
if (caller_perms().wizard)
  {?kill = 0} = args;
  if (kill)
    count = 1;
    while (this.processing && (count < 11))
      suspend(1);
      count = count + 1;
    endwhile
    `kill_task(this.run_task) ! ANY';
  else
    if ($code_utils:task_valid(this.run_task) && (task_id() != this.run_task))
      return 1;
    else
      "do scheduled stuff here";
      delay = 3600;
      tt = time();
      to_do = this.scheduled_tasks;
      if (!to_do)
        `kill_task(this.run_task) ! ANY';
      else
        this.processing = 1;
        new_tasks = {};
        for event in (to_do)
          {repeat, object, verbo, runtime, owner, ?opargs = {}} = event;
          if (typeof(opargs) != LIST)
            "you may want to include some kind of error message surfacing here";
            opargs = {opargs};
          endif
          did_run = 0;
          if (tt >= runtime)
            fork (0)
              try
                "you can reset who the `player` is, possibly a generic $player_task_owner";
                set_task_perms(owner);
                if ($recycler:valid(object))
                  object:(verbo)(@opargs);
                else
                  "TODO: you may want to include some kind of error message handling here";
                endif
              except ecodes (ANY)
                "TODO: consider adding some error catching here on your server";
              endtry
            endfork
            if (repeat && (runtime = this:next_runtime(@event)))
              if (((runtime - time()) < 10) && (verbo != "tick"))
                "You may want to add some kind of error logging here";
              endif
              new_tasks = {@new_tasks, {repeat, object, verbo, runtime, owner, opargs}};
            endif
            "we no longer mark not processing at this point, now doing it just before we set the tasks prop below - J 3/29/20";
            "this.processing = 0";
          else
            new_tasks = {@new_tasks, event};
          endif
          ((ticks_left() < 2000) || (seconds_left() < 2)) && suspend(min($login:current_lag(), 10));
        endfor
      endif
      if (this.incoming_tasks)
        "add any tasks that were scheduled while this was running";
        new_tasks = {@new_tasks, @this.incoming_tasks};
        this.incoming_tasks = {};
      endif
      if (new_tasks)
        new_tasks = $list_utils:sort_alist_suspended(1, new_tasks, 4);
        delay = new_tasks[1][4] - time();
        delay = max(delay, 1);
        fork SCHED (delay)
          this:(verb)();
        endfork
        this.run_task = SCHED;
      endif
      this.processing = 0;
      this.scheduled_tasks = new_tasks;
      return 1;
    endif
  endif
else
  return E_PERM;
endif
.

@args #112:"schedule_for" this none this
@program #112:schedule_for
"schedule_for( INT time, OBJ object, STR verbname [, LIST optional_args]) =>";
"add verbname on object to be run at time seconds from now.  If optional_args is given, then they are passed to the verb when it's run";
{runtime, object, verbname, ?opargs = {}} = args;
task = {0, object, verbname, time() + runtime, caller_perms(), opargs};
if (this.processing)
  this.incoming_tasks = {@this.incoming_tasks, task};
else
  this.scheduled_tasks = {@this.scheduled_tasks, task};
endif
this:halt();
return this:start();
.

@args #112:"schedule_at" this none this
@program #112:schedule_at
"schedule_at( INT time, OBJ object, STR verbname [, LIST optional_args]) =>";
"add verbname on object to be run at time (as time() returns).  If optional_args is given, then they are passed to the verb when it's run";
{runtime, object, verbname, ?opargs = {}} = args;
repeat = 0;
tasks = this.scheduled_tasks;
objs = $list_utils:slice(tasks, 2);
verbage = $list_utils:slice(tasks, 3);
i = object in objs;
if (i && (verbage[i] == verbname))
  return E_INVARG;
endif
task = {repeat, object, verbname, runtime, caller_perms(), opargs};
if (this.processing)
  this.incoming_tasks = {@this.incoming_tasks, task};
else
  this.scheduled_tasks = {@this.scheduled_tasks, task};
endif
this:halt();
return this:start();
.

@args #112:"schedule_every" this none this
@program #112:schedule_every
"schedule_every( VARIES interval, OBJ object, STR verbname [, LIST optional_args]) =>";
"add verbname on object to be run every interval seconds from now.  If optional_args is given, then they are passed to the verb when it's run";
"";
"interval args supported:";
"  INT - fixed number of seconds";
"  STR - hh:mm:ss time less than 24 hours, every day as close to this time as possible";
"  {INT minimum, INT range} - between minimum and minimum+range seconds";
"  {OBJ timing_obj, STR verbname, [LIST additional args]} - calculated by call to timing_obj:verbname";
{repeat, object, verbname, ?opargs = {}, ?allow_duplicates = 0} = args;
"throw E_INVARG if the repeat arg is bad";
this:check_repeat_args(repeat);
runtime = time();
if (!allow_duplicates)
  tasks = this.scheduled_tasks;
  objs = $list_utils:slice(tasks, 2);
  verbage = $list_utils:slice(tasks, 3);
  i = object in objs;
  if (i && (verbage[i] == verbname))
    return E_INVARG;
  endif
endif
task = {repeat, object, verbname, runtime, caller_perms(), opargs};
if (this.processing)
  this.incoming_tasks = {@this.incoming_tasks, task};
else
  this.scheduled_tasks = {@this.scheduled_tasks, task};
endif
this:halt();
return this:start();
.

@args #112:"start halt" this none this
@program #112:start
"halt => stops scheduler";
"start => starts scheduler";
kill = 0;
if (verb == "halt")
  kill = 1;
endif
return this:run_scheduled(kill);
.

@args #112:"look_self" this none this
@program #112:look_self
pass(@args);
this:display_schedule();
.

@args #112:"remove_scheduled" this none this
@program #112:remove_scheduled
"remove_scheduled( OBJ object, STR verbname, LIST iargs = \"THIS IS A NULL VALUE\") => remove object:verbname from scheduled tasks to be run.";
" Returns 1 if the task was successfully removed, 0 if it wasn't";
" Returns E_PERM if the caller is not the owner and not a wizard";
{object, ?verbname = "", ?iargs = "THIS IS A NULL VALUE"} = args;
cp = caller_perms();
tasks = this.scheduled_tasks;
killed = 0;
for x in (tasks)
  $command_utils:suspend_if_needed(0);
  if (((x[2] == object) && ((verbname == "") || (x[3] == verbname))) && ((iargs == "THIS IS A NULL VALUE") || (x[6] == iargs)))
    if (((cp == this.owner) || cp.wizard) || (cp == x[5]))
      this.scheduled_tasks = setremove(this.scheduled_tasks, x);
      killed = killed + 1;
    else
      return E_PERM;
    endif
  endif
endfor
tasks = this.incoming_tasks;
for x in (tasks)
  $command_utils:suspend_if_needed(0);
  if (((x[2] == object) && ((verbname == "") || (x[3] == verbname))) && ((iargs == "THIS IS A NULL VALUE") || (x[6] == iargs)))
    if (((cp == this.owner) || cp.wizard) || (cp == x[5]))
      this.incoming_tasks = setremove(this.incoming_tasks, x);
      killed = killed + 1;
    else
      return E_PERM;
    endif
  endif
endfor
return killed;
.

@args #112:"jesus" this none this
@program #112:jesus
if (this.jesus_running)
  for x in (this.kill_tasks)
    stack = `task_stack(x, 1) ! E_INVARG => {}';
    if (stack && ((stack[1][1] == $command_utils) && ((stack[1][2] == "sin") || (stack[1][2] == "suspend_if_needed"))))
      `kill_task(x) ! ANY';
      this.killed_tasks = setadd(this.killed_tasks, {stack, time(), connected_players()});
      try
        prefix = " ";
        for y in (stack)
          {c_this, c_verb, c_progger, c_verbloc, c_player, c_line} = y;
          prefix = prefix + ".";
        endfor
      except e (ANY)
        "can't risk dumping this error anywhere";
      endtry
    endif
    this.kill_tasks = setremove(this.kill_tasks, x);
  endfor
  for x in (queued_tasks())
    if (x[7] == "suspend_if_needed sin")
      this.kill_tasks = setadd(this.kill_tasks, x[1]);
    endif
  endfor
endif
fork (60)
  this:jesus();
endfork
.

@args #112:"is_scheduled" this none this
@program #112:is_scheduled
":is_scheduled(object OBJ, verbname STR) => returns 1 if the object:verbname is scheduled, 0 if it's not";
{objt, verbname} = args;
for tasks in ({this.scheduled_tasks, this.incoming_tasks})
  for x in (tasks)
    $command_utils:suspend_if_needed(0);
    if ((x[2] == objt) && (x[3] == verbname))
      return 1;
    endif
  endfor
endfor
return 0;
.

@args #112:"match" this none this
@program #112:match
command = callers()[$][2];
if (command in {"look", "l"})
  filter = args[1];
  thing = $string_utils:literal_object(filter, player);
  if (!$recycler:valid(thing))
    thing = $match_utils:match(filter, $list_utils:slice(this.scheduled_tasks, 2));
    if (!$recycler:valid(thing))
      return thing;
    endif
  endif
  this:look_self(thing);
  return "null";
endif
return pass(@args);
.

@args #112:"when_scheduled" this none this
@chown #112:when_scheduled #98
@program #112:when_scheduled
{objt, verbname} = args;
for tasks in ({this.scheduled_tasks, this.incoming_tasks})
  for x in (tasks)
    $command_utils:suspend_if_needed(0);
    if ((x[2] == objt) && (x[3] == verbname))
      return x[4];
    endif
  endfor
endfor
return 0;
.

@args #112:"next_runtime" this none this
@chown #112:next_runtime #98
@program #112:next_runtime
":next_runtime(@scheduled_task) => runtime INT (time value)";
" given a scheduled task, calculates the next runtime";
{repeat, object, verbo, runtime, owner, ?opargs = {}} = args;
repeat_type = typeof(repeat);
if (repeat)
  if (repeat_type == INT)
    "static repeat interval, the usual (integer number of seconds)";
    return time() + repeat;
  elseif (repeat_type == STR)
    "dynamic interval based on $time_utils:seconds_until_time";
    offset = $time_utils:seconds_until_time(repeat);
    return time() + ((offset <= 0) ? offset + $time_utils.day | offset);
  elseif (repeat_type == LIST)
    "dynamic interval";
    {interval_obj, interval_verb, ?more_args = {}} = repeat;
    if ((typeof(interval_obj) == INT) && (typeof(interval_verb) == INT))
      "simple random interval between two values {interval_minimum, interval_range} {10, 5} = 10 to 15 seconds";
      return (time() + interval_obj) + random(interval_verb);
    else
      "calculated interval by calling a specified verb {interval_obj, interval_verbo} {#1234, \"calc_verbo_runtime\"} = some number of seconds";
      try
        return time() + interval_obj:(interval_verb)(object, verbo, opargs, more_args);
      except e (ANY)
        "TODO: add some error logging here for your server";
        return 0;
      endtry
    endif
  endif
endif
return 0;
.

@args #112:"check_repeat_args" this none this
@chown #112:check_repeat_args #98
@program #112:check_repeat_args
{repeat} = args;
typeis = typeof(repeat);
if (typeis == LIST)
  if ((!repeat) || (length(repeat) < 2))
    raise(E_INVARG, "repeat arg bad value", repeat);
  endif
  a = typeof(repeat[1]);
  b = typeof(repeat[2]);
  if (a == OBJ)
    if (b != STR)
      raise(E_INVARG, "repeat arg object:verb not right", repeat);
    elseif (!$object_utils:has_callable_verb(repeat[1], repeat[2]))
      raise(E_INVARG, "repeat arg object:verb is not callable/exist", repeat);
    endif
  elseif (a == INT)
    if (b != INT)
      raise(E_INVARG, "repeat arg range not right", repeat);
    endif
  else
    raise(E_INVARG, "repeat arg range not right", repeat);
  endif
elseif (typeis == STR)
  try
    $time_utils:seconds_until_time(repeat);
  except e (ANY)
    raise(E_INVARG, "repeat arg specific time string not in hh:mm:ss format", repeat);
  endtry
elseif (typeis != INT)
  raise(E_INVARG, "repeat arg type invalid", repeat);
endif
.

@args #112:"describe_repeat" this none this
@program #112:describe_repeat
{repeat} = args;
if (!repeat)
  return "";
elseif (typeof(repeat) == INT)
  return $string_utils:from_seconds(repeat);
elseif (typeof(repeat) == LIST)
  if (typeof(repeat[1]) == INT)
    return tostr(repeat[1], " to ", repeat[1] + repeat[2], " seconds");
  else
    return tostr(repeat[1], ":", repeat[2]);
  endif
elseif (typeof(repeat) == STR)
  return repeat;
endif
return "bad args";
.

@args #112:"display_schedule" this none this
@program #112:display_schedule
":display_schedule(?LIST tasks) => none";
"display the scheduled tasks that are passed in, or the entire schedule if none passed in";
{?to_do = this.scheduled_tasks} = args;
filter = $nothing;
if (args && (typeof(args[1]) == OBJ))
  filter = args[1];
endif
player:tell("Current Time: ", ctime());
running = $code_utils:task_valid(this.run_task) ? "YES" | "NO";
player:tell("Currently Running: ", running);
su = $string_utils;
head = tostr(su:left("Next Run Time", 30), su:left("Repeat", 25), su:left("Owner", 10), "Verb");
head2 = tostr(su:left("-------------", 30), su:left("------", 25), su:left("-----", 10), "----");
display = {head, head2};
toggle = 1;
decaying = {};
for event in (to_do)
  {repeat, object, verbo, runtime, owner, ?opargs = {}} = event;
  if (verbo == "item_decay")
    decaying = setadd(decaying, object);
    continue;
  endif
  if ((filter == $nothing) || $object_utils:isa(object, filter))
    rep_str = this:describe_repeat(repeat);
    thing = (references = $code_utils:core_references(object)) ? "$" + references[1] | tostr(object);
    msg = tostr(su:left(ctime(runtime), 30)[1..30], su:left(rep_str, 25)[1..25], su:left(owner.name, 10)[1..10], thing, ":", verbo, "(", toliteral(opargs)[2..$ - 1], ")");
    display = {@display, msg};
    toggle = !toggle;
  endif
  ((ticks_left() < 2000) || (seconds_left() < 2)) && suspend(min($login:current_lag(), 10));
endfor
player:tell("Current scheduled tasks:");
player:tell(su:space(90, "*"));
player:tell_lines(display);
player:tell(su:space(90, "*"));
if (decaying)
  player:tell("Decaying Items [aka - object:item_decay()]");
  player:tell($string_utils:english_list(decaying));
endif
player:tell("List too long? 'help $scheduler' to view @scheduled options to prune it down.");
.

@program $code_utils:core_references
{thing} = args;
refs = {};
for ref in (properties(#0))
  $command_utils:suspend_if_needed(0);
  if (#0.(ref) == thing)
    refs = {@refs, ref};
  endif
endfor
return refs;
.

@program me:@scheduled
"@scheduled => varying list of scheduled tasks";
"$scheduler.scheduled_tasks => {MIXED interval, OBJ object, STR verbname, INT next_run, OBJ scheduled_by, LIST verb_args}";
if (!args)
  "default behavior, skip all the stuff we don't need";
  return $scheduler:display_schedule();
endif
scheduled_repeat = {};
scheduled_soon = {};
scheduled_later = {};
scheduled_norepeat = {};
scheduled_by_list = {};
scheduled_obj_list = {};
current_time = time();
rescheduled_by_interval = {};
rescheduled_by_verb = {};
rescheduled_by_timestring = {};
rescheduled_by_seconds = {};
scheduled_verbs = {};
scheduled_without = {};
"we will track by obj&verb and up a counter each time we find one";
"if there are no duplicates than it should be a list with counters all at 1";
possible_duplicates = {};
"5 minutes is 'soon'";
five_min = $time_utils.minute * 5;
soon = current_time + five_min;
"handle an exclusion case";
if (args[1] == "without")
  verb_to_exclude = args[2];
else
  verb_to_exclude = 0;
endif
for task in ($scheduler.scheduled_tasks)
  {interval, object, verbname, next_run, scheduled_by, verb_args} = task;
  if (verb_to_exclude && verbname != verb_to_exclude)
    scheduled_without = {@scheduled_without, task};
  endif
  if (!interval)
    "norepeat";
    scheduled_norepeat = {@scheduled_norepeat, task};
  else
    "repeat";
    scheduled_repeat = {@scheduled_repeat, task};
  endif
  if (next_run <= soon)
    "runs soon";
    scheduled_soon = {@scheduled_soon, task};
  else
    "runs later";
    scheduled_later = {@scheduled_later, task};
  endif
  "track who scheduled the task";
  if (index = scheduled_by in $list_utils:slice(scheduled_by_list))
    "already have a list";
    scheduled_by_list[index][2] = {@scheduled_by_list[index][2], task};
  else
    scheduled_by_list = {@scheduled_by_list, {scheduled_by, {task}}};
  endif
  "track the object the task is scheduled for";
  if (index = object in $list_utils:slice(scheduled_obj_list))
    "already have a list";
    scheduled_obj_list[index][2] = {@scheduled_obj_list[index][2], task};
  else
    scheduled_obj_list = {@scheduled_obj_list, {object, {task}}};
  endif
  if (interval)
    type_of_interval = typeof(interval);
    if (type_of_interval == INT)
      rescheduled_by_seconds = {@rescheduled_by_seconds, task};
    elseif (type_of_interval == STR)
      rescheduled_by_timestring = {@rescheduled_by_timestring, task};
    elseif (type_of_interval == LIST)
      if (typeof(interval[1]) == INT)
        rescheduled_by_interval = {@rescheduled_by_interval, task};
      elseif (typeof(interval[1]) == OBJ)
        rescheduled_by_verb = {@rescheduled_by_verb, task};
      endif
    endif
  endif
  "keep track of scheduled tasks that could be duplicates";
  hash = tostr(object, verbname);
  if (index = hash in $list_utils:slice(possible_duplicates))
    "already have an entry";
    possible_duplicates[index][2] = possible_duplicates[index][2] + 1;
    possible_duplicates[index][3] = {@possible_duplicates[index][3], task};
  else
    "no entry, so we add one and set the count to 1";
    possible_duplicates = {@possible_duplicates, {hash, 1, {task}}};
  endif
  "track the verbs that are scheduled";
  if (index = verbname in $list_utils:slice(scheduled_verbs))
    "already have a list";
    scheduled_verbs[index][2] = {@scheduled_verbs[index][2], task};
  else
    scheduled_verbs = {@scheduled_verbs, {verbname, {task}}};
  endif
endfor
if (argstr == "repeat")
  $scheduler:display_schedule(scheduled_repeat);
elseif (argstr == "norepeat")
  $scheduler:display_schedule(scheduled_norepeat);
elseif (argstr == "soon")
  $scheduler:display_schedule(scheduled_soon);
elseif (argstr == "later")
  $scheduler:display_schedule(scheduled_later);
elseif (argstr == "interval")
  $scheduler:display_schedule(rescheduled_by_interval);
elseif (argstr == "verb")
  $scheduler:display_schedule(rescheduled_by_verb);
elseif (argstr == "timestring")
  $scheduler:display_schedule(rescheduled_by_timestring);
elseif (argstr == "seconds")
  $scheduler:display_schedule(rescheduled_by_seconds);
elseif (argstr == "duplicates")
  dupes = {};
  for entry in (possible_duplicates)
    if (entry[2] > 1)
      dupes = {@dupes, @entry[3]};
    endif
  endfor
  $scheduler:display_schedule(dupes);
elseif (index(argstr, "without "))
  $scheduler:display_schedule(scheduled_without);
elseif (index(argstr, "by "))
  possible_obj = toobj(args[2]);
  "purposefully using valid and not $recycler:valid because a recycled obj could be a scheduler";
  if (!valid(possible_obj) || possible_obj == $sysobj)
    possible_obj = $string_utils:match_player(args[2]);
    if (possible_obj in {#-3, #-2})
      return player:tell("Invalid usage: try @scheduled by <obj or name>");
    endif
  endif
  if (index = possible_obj in $list_utils:slice(scheduled_by_list))
    $scheduler:display_schedule(scheduled_by_list[index][2]);
  else
    return player:tell("No scheduled tasks by obj: ", $su:nn(possible_obj), " were found.");
  endif
elseif (index(argstr, "for "))
  if (index(args[2], "$") == 1)
    possible_obj = $sysobj.(args[2][2..$]);
  else
    possible_obj = toobj(args[2]);
  endif
  "purposefully using valid and not $recycler:valid because a recycled obj could be a scheduler";
  if (!valid(possible_obj) || possible_obj == $sysobj)
    possible_obj = $string_utils:match_player(args[2]);
    if (possible_obj in {#-2, #-3})
      "we are trying to view a verbname and not an obj";
      verbname = args[2];
      if (index = verbname in $list_utils:slice(scheduled_verbs))
        $scheduler:display_schedule(scheduled_verbs[index][2]);
      else
        return player:tell("No scheduled tasks with verbname: ", verbname, " were found.");
      endif
    endif
  endif
  if (index = possible_obj in $list_utils:slice(scheduled_obj_list))
    $scheduler:display_schedule(scheduled_obj_list[index][2]);
  endif
else
  player:tell("Invalid paramters, help $scheduler for more info on how to use this command.");
endif
.
