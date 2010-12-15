open Batteries

type 'a bloom_filter = { hashers : ('a -> int) list;
                         bitset : BitSet.t;
                         size: int }

let create hashers size =  { hashers = hashers;
                             bitset = BitSet.create size;
                             size = size }

let mem value bloom =
  let mem' hasher =
    let pos = (hasher value) mod bloom.size in
    BitSet.is_set bloom.bitset pos
  in List.for_all mem' bloom.hashers

let insert value bloom =
  let positions =
    List.map (fun hasher -> (hasher value) mod bloom.size)
      bloom.hashers
  in List.iter (fun pos -> BitSet.set bloom.bitset pos) positions
