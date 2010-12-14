open Batteries

type color = Red | Black
type 'a tree = Empty | Tree of color * 'a tree * 'a * 'a tree

let empty = Empty

let rec mem x = function
  | Empty             -> false
  | Tree (_, a, y, b) ->
    if x < y then mem x a
    else if x = y then true
    else mem x b

let balance color a x b =
  match color, a, x, b with
    | Black, Tree (Red, Tree (Red, a, x, b), y, c), z, d ->
      Tree (Red, Tree (Black, a, x, b), y, Tree (Black, c, z, d))
    | Black, Tree (Red, a, x, Tree (Red, b, y, c)), z, d ->
      Tree (Red, Tree (Black, a, x, b), y, Tree (Black, c, z, d))
    | Black, a, x, Tree (Red, Tree (Red, b, y, c), z, d) ->
      Tree (Red, Tree (Black, a, x, b), y, Tree (Black, c, z, d))
    | Black, a, x, Tree (Red, b, y, Tree (Red, c, z, d)) ->
      Tree (Red, Tree (Black, a, x, b), y, Tree (Black, c, z, d))
    | _ -> Tree (color, a, x, b)

let insert x tree =
  let make_black = function
    | Tree (_, a, y, b) -> Tree (Black, a, y, b)
    | Empty             -> raise (Failure "Empty tree passed") in
  let rec insert' = function
    | Empty -> Tree (Red, Empty, x, Empty)
    | Tree (color, a, y, b) ->
      if      x < y then balance color (insert' a) y b
      else if x = y then Tree (color, a, y, b)
      else               balance color a y (insert' b)
  in
  make_black (insert' tree)
