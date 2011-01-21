open Batteries

type btree =
  | Node of int list * btree list

let min_degree = 2
let empty = Node ([], [])

let is_leaf = function
  | Node (_, children) -> List.is_empty children

let is_full = function
  | Node (keys, _) -> List.length keys = 2 * min_degree - 1

let rec find tree key = match tree with
  | Node ([], []) -> None
  | Node (keys, children) ->
    if List.mem key keys then Some key
    else
      let rec find' ks cs = match ks, cs with
        | [], [] -> None
        | [], [last] -> find last key
        | k :: ks', c :: cs' ->
          if key < k then find c key else find' ks' cs'
        | _ -> raise (Failure "Not expected to reach this clause")
      in find' keys children

let mem tree key = match find tree key with
  | Some _ -> true
  | None   -> false

let split_full = function
  | Node (keys, children) ->
    let lkeys, rkeys = List.split_at (min_degree - 1) keys in
    let median = List.hd rkeys in
    let rkeys = List.tl rkeys in
    match children with
      | [] -> Node ([median], [Node (lkeys, []); Node (rkeys, [])])
      | _ -> let i = List.length lkeys in
             let lchildren, rchildren = List.split_at i children in
             Node ([median], [Node (lkeys, lchildren);
                              Node (rkeys, rchildren)])

let split_by key keys = List.partition (fun k -> key > k) keys

let rec insert tree key = match tree with
  | Node ([], []) -> Node ([key], [])
  | Node (keys, []) ->
    if is_full tree
    then let tree = split_full tree in
         insert tree key
    else let lkeys, rkeys = split_by key keys in
         Node (lkeys @ key :: rkeys, [])
  | Node (keys, children) ->
    if is_full tree
    then let tree = split_full tree in
         insert tree key
    else let lkeys, rkeys = split_by key keys in
         let i = List.length lkeys in
         let lchildren, rchildren = List.split_at i children in
         let next = List.hd rchildren in
         let rchildren = List.tl rchildren in
         Node (keys, lchildren @ (insert next key) :: rchildren)
