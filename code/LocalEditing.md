# Local Editing (in browser or VMOO or other supported client)

## Update

If you are using a ToastCore or even LambdaCore based database, you may be able to just do `@edit-options +local` and have local editing working without needing to do really any of the stuff in this doc.

## Contributing

The code scanner covers some of our best practices for MOO coding on [Sindome](https://www.sindome.org/). If you have your own that you think would be useful to the community please feel free to put in a pull request!

## Requirements
This code was tested on stock LambdaMOO 1.8.1 running the lastest LambdaCore.db. If your DB is based off of that it should work with no changes needed.

This code will allow you to support Local Editing via the Dome-Client webclient, and also probably VMOO and other clients that support local editing (though it may take some massaging). 

## Installation 

You may need to change the obj# from #480 to whatever yours is when you create the web client package. This should be a generic package, but it doesn't really need to be. Any object will work.

This code actually works best if it's on $verb_editor but I've broken it out into it's own package and copied a lot of the $verb_editor verbs that were needed over to the new package, since some of the stock LambdaCore verb editor verbs require $verb_editor to be the caller, and otherwise you'd have to modify $verb_editor which you might not have permission to do.

When all of this code is in place you should be able to use the dome-client (http://pubclient.sindome.org/) to edit verbs in a modal window use @edit obj:verb

This can also be modified to support editing properties if you want.

```
;;#480.("key") = 0
;;#480.("aliases") = {"Webclient Package"}
;;#480.("description") = "This is a placeholder parent for all the $..._utils packages, to more easily find them and manipulate them. At present this object defines no useful verbs or properties. (Filfre.)"
;;#480.("object_size") = {0, 0}

@args #480:"local_editing_info" this none this
@program #480:local_editing_info
{object, vname, code} = args;
if (typeof(vname) == LIST)
  vargs = tostr(" ", vname[2], " ", $code_utils:short_prep(vname[3]), " ", vname[4]);
  vname = vname[1];
else
  vargs = "";
endif
name = tostr(object:name(), ":", vname);
"... so the next 2 lines are actually wrong, since verb_info won't";
"... necessarily retrieve the correct verb if we have more than one";
"... matching the given same name; anyway, if parse_invoke understood vname,";
"... so will @program.  I suspect these were put here because in the";
"... old scheme of things, vname was always a number.";
"vname = strsub($string_utils:explode(verb_info(object, vname)[3])[1], \"*\", \"\")";
"vargs = verb_args(object, vname)";
"";
return {name, code, tostr("@program ", object, ":", vname, vargs)};
"[Modified by Slither (#479) Wed Jun 20 19:37:16 2018 WEST].";
.

@args #480:"invoke" this none this
@program #480:invoke
":invoke(...)";
"to find out what arguments this verb expects,";
"see this editor's parse_invoke verb.";
new = args[1];
spec = this:parse_invoke(@args);
if (typeof(spec) == LIST)
  if ($object_utils:has_verb(this, "local_editing_info") && (info = this:local_editing_info(@spec)))
    player:tell("Invoking local editor");
    this:invoke_local_editor(@info);
  else
    player:tell("This is for editing in a web client, if you don't wanna do that, use a different verb.");
  endif
endif
"[Modified by Slither (#479) Wed Jun 20 20:03:23 2018 WEST].";
.

@args #480:"parse_invoke" this none this
@program #480:parse_invoke
":parse_invoke(string,v)";
"  string is the commandline string to parse to obtain the obj:verb to edit";
"  v is the actual command verb used to invoke the editor";
" => {object, verbname, verb_code} or error";
vref = $string_utils:words(args[1]);
if ((!vref) || (!(spec = $code_utils:parse_verbref(vref[1]))))
  player:tell("Usage: ", args[2], " object:verb");
  return;
endif
if (argspec = listdelete(vref, 1))
  if (typeof(pas = $code_utils:parse_argspec(@argspec)) == LIST)
    if (pas[2])
      player:tell("I don't understand \"", $string_utils:from_list(pas[2], " "), "\"");
      return;
    endif
    argspec = {@pas[1], "none", "none"}[1..3];
    argspec[2] = $code_utils:full_prep(argspec[2]) || argspec[2];
  else
    player:tell(pas);
    return;
  endif
endif
player:tell(toliteral(argspec));
if (!$command_utils:object_match_failed(object = player:my_match_object(spec[1], $verb_editor:get_room(player)), spec[1]))
  vnum = $code_utils:find_verb_named(object, vname = spec[2]);
  if (argspec)
    while (vnum && (verb_args(object, vnum) != argspec))
      vnum = $code_utils:find_verb_named(object, vname, vnum + 1);
    endwhile
  endif
  if (vnum)
    code = this:fetch_verb_code(object, vnum);
  else
    code = E_VERBNF;
  endif
  if (typeof(code) == ERR)
    player:tell((code != E_VERBNF) ? code | "That object does not define that verb", argspec ? " with those args." | ".");
    return code;
  else
    return {object, argspec ? {vname, @argspec} | vname, code};
  endif
endif
return 0;
"[Modified by Slither (#479) Wed Jun 20 19:58:14 2018 WEST].";
.

@args #480:"fetch_verb_code" this none this
@program #480:fetch_verb_code
set_task_perms(player);
return `verb_code(args[1], args[2], !player:edit_option("no_parens")) ! ANY';
"[Modified by Slither (#479) Wed Jun 20 19:57:52 2018 WEST].";
.

@args #480:"invoke_local_editor" this none this
@program #480:invoke_local_editor
":invoke_local_editor(name, text, upload)";
"Spits out the magic text that invokes the local editor in the player's client.";
"NAME is a good human-readable name for the local editor to use for this particular piece of text.";
"TEXT is a string or list of strings, the initial body of the text being edited.";
"UPLOAD, a string, is a MOO command that the local editor can use to save the text when the user is done editing.  The local editor is going to send that command on a line by itself, followed by the new text lines, followed by a line containing only `.'.  The UPLOAD command should therefore call $command_utils:read_lines() to get the new text as a list of strings.";
if (caller != this)
  return;
endif
{name, text, upload} = args;
if (typeof(text) == STR)
  text = {text};
endif
this:local_instruction(name, upload);
":dump_lines() takes care of the final `.' ...";
for line in ($command_utils:dump_lines(text))
  notify(player, line);
endfor
"[Modified by Slither (#479) Wed Jun 20 20:00:34 2018 WEST].";
.

@args #480:"local_instruction" this none this
@program #480:local_instruction
{label, ?upload = "none"} = args;
player:tell("#$# edit name: ", label, " upload: ", upload);
.

```

To access local editing you need a verb on yourself (or on a parent Programmer bit)

```
@program #479:@edit   any any any
"Calls the verb editor on verbs, the note editor on properties, and on anything else assumes it's an object for which you want to edit the .description.";
len = player.linelen;
"if (player:edit_option(\"local\"))";
"player.linelen = abs(len) * -1;";
"endif";
if (!args)
  (player in $note_editor.active ? $note_editor | $verb_editor):invoke(dobjstr, verb);
elseif ($code_utils:parse_verbref(args[1]))
  if (player.programmer)
    #480:invoke(argstr, verb);
    player:tell("invoke done");
  else
    player:notify("You need to be a programmer to do this.");
    player:notify("If you want to become a programmer, talk to a wizard.");
    return;
  endif
elseif ($list_editor:is_valid(dobjstr))
  $list_editor:invoke(dobjstr, verb);
else
  $note_editor:invoke(dobjstr, verb);
endif
"player.linelen = len;";
"[Modified by Slither (#479) Wed Jun 20 20:03:49 2018 WEST].";
.
```
