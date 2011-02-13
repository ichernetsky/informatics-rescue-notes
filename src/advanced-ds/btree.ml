open Batteries

type btree = { degree : int; tree : tree }
and tree =
  | Node of int list * tree list
  | Leaf of int list

let create degree =
  if degree < 2
  then raise (Invalid_argument "degree < 2")
  else { degree = degree; tree = Leaf [] }

let find btree key =
  let rec find' = function
    | Leaf [] -> None
    | Leaf keys ->
      if List.mem key keys then Some key else None
    | Node (keys, children) ->
      if List.mem key keys then Some key
      else let rec move_right ks cs =
             match ks, cs with
               | [], [c] -> find' c
               | k :: ks, c :: cs ->
                 if k > key then find' c else move_right ks cs
               | _ -> raise (Failure "Hot we got here?!")
           in move_right keys children
  in find' btree.tree

let is_full tree degree = match tree with
  | Node (keys, _) | Leaf keys ->
    List.length keys = 2 * degree - 1

let split_at_center list =
  let center = (List.length list) / 2 in
  let left, right = List.split_at center list in
  let middle, right = List.hd right, List.tl right in
  left, middle, right

let split_leaf = function
  | Leaf keys -> let left, middle, right = split_at_center keys in
                 Node ([middle], [Leaf left; Leaf right])
  | _ -> raise (Invalid_argument "leaf")

let split_node = function
  | Node (keys, children) ->
    let left, middle, right = split_at_center keys in
    let i = (List.length left) + 1 in
    let left_children, right_children = List.split_at i children in
    Node ([middle], [Node (left,  left_children);
                     Node (right, right_children)])
  | _ -> raise (Invalid_argument "node")

let split_full tree =
  match tree with
    | Leaf _ -> split_leaf tree
    | Node _ -> split_node tree

let split_with key = List.partition (fun k -> key > k)

let rec insert btree key =
  let rec insert' tree =
    match tree with
      | Leaf [] -> Leaf [key]
      | Leaf keys ->
        if is_full tree btree.degree
        then let tree = split_full tree in insert' tree
        else let left, right = split_with key keys in
             Leaf (left @ key :: right)
      | Node (keys, children) ->
        if is_full tree btree.degree
        then let tree = split_full tree in insert' tree
        else let left_keys, right_keys = split_with key keys in
             let i = List.length left_keys in
             let left_children, right_children = List.split_at i children in
             let next = List.hd right_children in
             let right_children = List.tl right_children in
             if is_full next btree.degree
             then match split_full next with
               | Node ([median], [left; right]) ->
                 let left, right =
                   if key <= median then (insert' left, right)
                   else (left, insert' right) in
                 Node (left_keys @ median :: right_keys,
                       left_children @ left :: right :: right_children)
               | _ -> raise (Failure "How we got here?!")
             else Node (keys, left_children @ (insert' next) :: right_children)
  in { btree with tree = insert' btree.tree }
