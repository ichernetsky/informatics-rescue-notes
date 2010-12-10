open Batteries

type 'a bloom_filter = { hash_funcs : ('a -> int) list;
                         bitset : BitSet.t;
                         size: int }

let make hash_funcs size =  { hash_funcs = hash_funcs;
                              bitset = BitSet.create size;
                              size = size }

let present bfilter value =
  let present_one hash_func =
    let position = (hash_func value) mod bfilter.size in
    BitSet.is_set bfilter.bitset position
  in
  List.for_all present_one bfilter.hash_funcs

let insert bfilter value =
  let positions =
    List.map (fun hash_func -> (hash_func value) mod bfilter.size)
      bfilter.hash_funcs
  in
  List.iter (fun position -> BitSet.set bfilter.bitset position ) positions
