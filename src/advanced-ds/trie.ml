open Batteries
open List

type 'a trie = { value : 'a option;
                 children : (char * 'a trie) list }

let make_node value children =
  { value = value; children = children }

let make_empty_node () = make_node None []
let make_root () = make_empty_node ()

let lookup pairs key =
  try Some (assoc key pairs) with
    | Not_found -> None

let find trie key =
  let rec find' trie key =
    match key with
      | [] -> trie.value
      | letter :: rest ->
        match lookup trie.children letter with
          | None -> None
          | Some trie' -> find' trie' rest in
  find' trie (String.to_list key)

let exists trie key =
  match find trie key with
    | None -> false
    | Some _ -> true

let insert trie key value =
  let rec insert' trie key =
    match key with
      | [] -> {trie with value = Some value}
      | letter :: rest ->
        match lookup trie.children letter with
          | None ->
            let node = make_empty_node () in
            {trie with children = (letter, insert' node rest) :: trie.children}
          | Some trie' ->
            let filtered = filter (fun pair -> (fst pair) <> letter) trie.children in
            {trie with children = (letter, insert' trie' rest) :: filtered}
  in
  insert' trie (String.to_list key)

let remove trie key =
  let rec remove' trie key =
    match key, trie.value, trie.children with
      | [], _, [] -> None
      | [], _, _ -> Some {trie with value = None}
      | letter :: rest, value, children ->
        match lookup children letter with
          | None -> Some trie
          | Some trie' ->
            let filtered = filter (fun pair -> (fst pair) <> letter) children in
            match remove' trie' rest with
              | None ->
                (match value, filtered with
                  | None, [] -> None
                  | _ -> Some {trie with children = filtered})
              | Some trie'' -> Some {trie with children = (letter, trie'') :: filtered}
  in
  match remove' trie (String.to_list key) with
    | None -> make_root ()
    | Some root -> root
