module Main
       where

import Prelude hiding (lookup, null)

import qualified Data.Map as Map
import qualified Data.Maybe as Maybe

data Trie k v = Trie (Maybe v) (Map.Map k (Trie k v))
                deriving Show

empty :: Ord k => Trie k v
empty = Trie Nothing Map.empty

null :: Ord k => Trie k v -> Bool
null (Trie Nothing children) = Map.null children
null _                       = False

lookup :: Ord k => [k] -> Trie k v -> Maybe v
lookup [] (Trie value _)          = value
lookup (k : ks) (Trie _ children) = do
  next <- Map.lookup k children
  lookup ks next

member :: Ord k => [k] -> Trie k v -> Bool
member k = Maybe.isJust . lookup k

insert :: Ord k => [k] -> v -> Trie k v -> Trie k v
insert [] value (Trie _ children) = Trie (Just value) children
insert (k : ks) value (Trie v children) =
  let next = Map.findWithDefault empty k children
      node = insert ks value next in
  Trie v $ Map.insert k node children

delete :: Ord k => [k] -> Trie k v -> Trie k v
delete [] (Trie _ children) = Trie Nothing children
delete (k : ks) trie@(Trie value children) =
  case Map.lookup k children of
    Just next ->
      let node = delete ks next in
      if null node
      then Trie value $ Map.delete k children
      else Trie value $ Map.insert k node children
    Nothing   -> trie

fromList :: Ord k => [([k], v)] -> Trie k v
fromList = foldl (\t (k, v) -> insert k v t) empty

deleteAll :: Ord k => [[k]] -> Trie k v -> Trie k v
deleteAll keys trie = foldl (flip delete) trie keys

main :: IO ()
main = do
  let keys = ["tea", "ted", "ten", "inn", "te", ""]
  let trie = fromList [ (k, True) | k <- keys ]
  putStrLn $ show trie
  putStrLn $ show $ deleteAll keys trie
