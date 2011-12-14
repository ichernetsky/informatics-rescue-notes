module Main
       where

data Color = R
           | B
           deriving Show

data Tree e = E
            | T Color (Tree e) e (Tree e)
            deriving Show

empty :: Tree e
empty = E

member :: Ord e => e -> Tree e -> Bool
member x E = False
member x (T _ a y b) | x <  y = member x a
                     | x == y = True
                     | x >  y = member x b

insert :: Ord e => e -> Tree e -> Tree e
insert x t = makeBlack (insert' t)
  where insert' E = T R E x E
        insert' (T color a y b) | x <  y = balance color (insert' a) y b
                                | x == y = T color a y b
                                | x >  y = balance color a y (insert' b)
        makeBlack (T _ a x b) = T B a x b

balance :: Color -> Tree e -> e -> Tree e -> Tree e
balance B (T R (T R a x b) y c) z d = T R (T B a x b) y (T B c z d)
balance B (T R a x (T R b y c)) z d = T R (T B a x b) y (T B c z d)
balance B a x (T R b y (T R c z d)) = T R (T B a x b) y (T B c z d)
balance B a x (T R (T R b y c) z d) = T R (T B a x b) y (T B c z d)
balance color a x b = T color a x b

fromList :: Ord e => [e] -> Tree e
fromList = foldl (flip insert) E

main :: IO ()
main = do
  putStrLn $ show $ fromList [9, 8, 7, 4, 5, 6, 3, 2, 1, 0]
