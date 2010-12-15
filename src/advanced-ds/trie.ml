open Batteries

type ('a, 'b) trie = { value    : 'a option;
                       children : ('b, ('a, 'b) trie) Map.t }

let empty = { value = None; children = Map.empty }

let rec find key trie = match key with
  | []           -> trie.value
  | head :: rest ->
    if Map.mem head trie.children then
      let next = Map.find head trie.children in
      find rest next
    else None

let mem key trie = match find key trie with
  | None -> false
  | Some _ -> true

let rec insert key value trie = match key with
  | [] -> {trie with value = Some value}
  | head :: rest ->
    if Map.mem head trie.children then
      let node = Map.find head trie.children in
      let node = insert rest value node in
      {trie with children = Map.add head node trie.children}
    else
      let node = insert rest value empty in
      {trie with children = Map.add head node trie.children}

let remove key trie =
  let rec remove' key trie =
    match key, Map.is_empty trie.children with
      | [], true  -> None
      | [], false -> Some {trie with value = None}
      | head :: rest, _ ->
        if Map.mem head trie.children then
          let next = Map.find head trie.children in
          let filtered = Map.remove head trie.children in
          match remove' rest next with
            | None ->
              (match trie.value, Map.is_empty filtered with
                | None, true -> None
                | _ -> Some {trie with children = filtered})
            | Some result ->
              Some {trie with children = Map.add head result filtered}
        else Some trie
  in match remove' key trie with
    | None -> empty
    | Some trie -> trie
