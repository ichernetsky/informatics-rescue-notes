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
  let bloom = {bloom with bitset = BitSet.copy bloom.bitset}
  and positions =
    List.map (fun h -> (h value) mod bloom.size)
      bloom.hashers in
  List.iter (fun p -> BitSet.set bloom.bitset p) positions;
  bloom
