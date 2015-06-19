let stm_segment_nb = 15;;

let compile_only = ref false;;
let output_file = ref None;;
let verbose = ref false;;
let pp_commands = ref [];;
let bc2asm_args = ref [];;
  
let spec =
  Arg.align [
      ("-c", Arg.Set compile_only,
       " Compile only");
      ("-o", Arg.String (fun o -> output_file := Some o),
       "<file.hex> Set output file");
      ("-pp", Arg.String (fun cmd -> pp_commands := cmd :: !pp_commands),
       "<command> Pipe sources through preprocessor <command>");
      ("-version", Arg.Unit (fun () -> print_endline Config.version ; exit 0),
       " Print version and exit");
      ("-verbose", Arg.Set verbose,
       " Verbose mode");
    ]
;;
  
let usage = 
  let me = Sys.argv.(0) in
  Printf.sprintf "\n\
		  \  Usages:\n\
		  \  %s [ <stm32> ] [ OCAMLC_OPTS ] [ OPTIONS ] -c <src.ml>  ...\n\
		  \  %s [ <stm32> ] [ OCAMLC_OPTS ] [ OPTIONS ] -c <src.mli> ...\n\
		  \  %s <stm32> [ OCAMLC_OPTS ] [ OPTIONS ] <src.ml>  ... \n\
		  \  %s <stm32> [ OCAMLC_OPTS ] [ OPTIONS ] <obj.cmo> ... \n\
		  \n\
		  Options :" me me me me
;;
  
let error msg =
  Printf.eprintf "Error : %s\n" msg;
  Arg.usage spec usage;
  exit 1;
;;
  
let stm32 =
  if Array.length Sys.argv < 2 then error "argument requested";
  let stm = Sys.argv.(1) in
  if List.mem stm Config.supported_stm32 then (
    incr Arg.current;
    Some stm
  ) else if String.contains stm '.' || String.contains stm '-' then (
    None
  ) else ( 
    error ("invalid or unsupported stm32 : " ^ stm)
  )
;;
  
let ml = ref [];;
let cmo = ref [];;
let ocamlc_options = ref [];;
  
let unknown arg =
  let get_arg () =
    incr Arg.current;
    if !Arg.current >= Array.length Sys.argv then
      error ("option `" ^ arg ^ "' needs an argument");
    Sys.argv.(!Arg.current)
  in
  if Filename.check_suffix arg ".ml" then
    ml := arg :: !ml
  else if Filename.check_suffix arg ".mli" then
    ml := arg :: !ml 
  else if Filename.check_suffix arg ".cmo" then
    cmo := arg :: !cmo
  else
    ocamlc_options := arg :: !ocamlc_options
;;
  
let execute prog args =
  let cmd = ref "" in
  if prog = Config.ocamlc then
    cmd := !cmd ^ Printf.sprintf "CAMLLIB=%s " (Filename.quote Config.libdir);
  cmd := !cmd ^ (Filename.quote prog);
  List.iter
    (fun arg -> cmd := Printf.sprintf "%s %s" !cmd (Filename.quote arg))
    args;
  if !verbose then Printf.printf "+ %s\n!" !cmd;
  match Sys.command !cmd with
  | 0 -> ()
  | n -> exit n
;;
  
  while !Arg.current <> Array.length Sys.argv do
    try
      Arg.parse_argv Sys.argv spec unknown usage;
    with
    | Arg.Help msg -> Printf.printf "%s" msg; exit 0;
    | Arg.Bad msg -> match Sys.argv.(!Arg.current) with
		     | "-c" | "-o" | "-verbose" | "-version" ->
						   Printf.eprintf "%s" msg; 
						   exit 2;
		     | arg -> unknown arg
  done;
  
  ml := List.rev !ml;
  cmo := List.rev !cmo;
  
  begin
    let heap_size_opt = 
      try
	let heap_size = int_of_string (List.assoc "-heap_size" !bc2asm_args) in
	let total_heap_size = 2 * heap_size in
	if heap_size < 0 || total_heap_size > stm_segment_nb - 1 then
	  error (Printf.sprintf "invalid heap size : %d" heap_size);
	Some heap_size
      with
      | Not_found -> None
      | Failure "int_of_string" -> error "invalid heap_size, not an integer"
    and stack_size_opt =
      try
	let stack_size = int_of_string (List.assoc "-stack_size" !bc2asm_args) in
	if stack_size < 0 || stack_size > stm_segment_nb - 2 then
	  error (Printf.sprintf "invalid stack size : %d\n" stack_size);
	Some stack_size
      with
      | Not_found -> None
      | Failure "int_of_string" -> error "invalid stack size, not an integer"
    in match (stack_size_opt, heap_size_opt) with
       | (Some stack_size, Some heap_size) ->
	  let total_heap_size = 2 * heap_size in
	  if stack_size + total_heap_size > stm_segment_nb then
	    error "incompatible stack/heap sizes";
       | _ -> ()
  end;
  
  let stm32 =
    match stm32 with
    | Some stm -> stm
    | None -> error "stm32 requested" in
  let ml_not_mli =
    List.filter (fun f -> Filename.check_suffix f ".ml") !ml in
  let rec last l =
    match l with
    | [] -> invalid_arg "Ocapic.last"
    | e::[] -> e
    | _::tl -> last tl in
  let verbose_opt = if !verbose then [ "-verbose" ] else [] in
  if !cmo = [] && ml_not_mli = [] then error ".ml or .cmo file(s) needed";
  let output_base =
    match !output_file with
    | None ->
       if ml_not_mli <> [] then
         Filename.chop_suffix (last ml_not_mli) ".ml"
       else
         Filename.chop_suffix (last !cmo) ".cmo"
    | Some f ->
       if not (Filename.check_suffix f ".asm") then
         error "output filename should end with .asm extension";
       Filename.chop_suffix f ".asm"
  in
  let bc2asm_args =
    List.fold_left (fun acc (a, v) -> a :: v :: acc) [] !bc2asm_args
  in
  execute Config.ocamlc
	  ("-custom" :: "-I" :: ("+" ^ stm32) :: "piclib.cma" :: !ocamlc_options @
             verbose_opt @ !cmo @ !ml @ [ "-o" ; output_base ]);
  execute Config.ocamlclean
	  (verbose_opt @ [ output_base ; "-o" ; output_base ]);
;;
