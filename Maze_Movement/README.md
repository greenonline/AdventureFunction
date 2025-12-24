# Maze movement

I have just discovered a follow up article by Graham Relf: [Practical Compµting, Sept 1982 - Maze movement](https://archive.org/details/practical-computing/PracticalComputing-1982-09/page/126/mode/2up).

[![Maze movement - page 1][1]][1]

[![Maze movement - page 2][2]][2]

[![Maze movement - page 3][3]][3

The code:

```none
Listing 1. Initiate table of displaced objects

Pseudo-code:

    L = length of table less one
    Dimension x column (L), y column (L), z column (L), code column (L), status column (L), pointer column (L)
    For row no = 0 to L
        code column (row no) = -1
        pointr column (row no) = -2
    next row no


BASIC:

    100 L = 00
    110 DIM OX(L), OY(L), OZ(L), OC(L), OS(L), OP(L)
    120 FOR R = 0 TO L
    130 OC(R) = -1 : OP(R) = -2
    140 NEXT R


Listing 2. Subroutine: Find given object at given location.

    Inputs:  Object code, X, Y, Z, W, L, table.
    Outputs: Entry row no ( = -1 if not found),
             previous row no in chain (= -1 if none).


Pseudo-code:

    Row no = W * L
    Previous row no in chain = -1
    Entry row no = -1
    Loop indefinitely        (to follow hash chain)
        If  code column (row no) = object code
        and x column (row no) = X
        and y column (row no) = Y
        and z column (row no) = Z
        then
            Entry row no = row no
            Return           (object found)
        end if
        If  pointer column (row no) >= 0
        then
            Previous row no in chain = row no
            Row no = pointer column (row no)
        else
            Return           (end of chain, not found)
        end if
    end loop

BASIC:

    5200  R = W * L  :  PR = -1  :  ER = -1
    5210  IF  OC(R)=CD  AND  OX(R)=X  AND  OY(R)=Y  AND  OZ(R)=Z
          THEN  ER = R  :  RETURN
    5220  IF  OP(R) >= 0  THEN PR = R : R = OP(R) : Go TO 5210
    5230  RETURN

Listing 3. Subroutine: Find all objects at given location.

    Inputs:  X, Y, Z, W, L, table.
    Outputs: Dependant on actions taken at x.

Pseudo-code:

    Row no = W * L
    Loop indefinitely        (to follow hash chain)
        If  code column (row no) <> -1
        and x column (row no) = X
        and y column (row no) = Y
        and z column (row no) = Z
        then
            * (Act on object found)
        end if
        If  pointer column (row no) < 0
        then
            Return           (end of chain)
        end if
        Row no = pointer column (row no)
    end loop


BASIC:

    4100  R = W * L
    4110  IF  OC(R)=-1  OR  OX(R)<>X  OR  OY(R)<>Y  OR  OZ(R)<>Z
          THEN 4170
    4120  )  Statements to act on object found (e.g. report it)
    ...   )
    4170  IF OP(R) >= 0  THEN  R = OP(R)  :  GO TO 4110
    4180  RETURN

Listing 4. Subroutine: Add object to table.

    Inputs:  Object code, object status, X, Y, Z, W, L, table.
    Outputs: Entry row no ( = -1 if not possible to add object),
             previous row no in chain ( = -1 if none), table.

NB: There are four possibilities:
    (1) The home location is free, so add there (giving previous
        row no = -1).
    (2) There is a free entry later in the chain, so add there
    (3) There is a free entry elsewhere in the table, so use that
        and extend the chain (staying close to home if possible).
    (4) There is no room in the table (gives entry row no = -1)


Pseudo-code:

    Row no = W * L
    Entry row no = -1
    Previous row in chain = -1
    Row no before home = row no - 1
    Row no after home = row no + 1
Loop:
    If  code column (row no) = -1
    then
        If  pointer column (row no) = -2
        then                 (case (1) or (2) )
            Pointer column (row no) = -1
        end if
        Go to  Put object
    end if
    If  pointer column (row no) <> -1
    then 
        Previous row no in chain = row no
        Row no = pointer column (row no)
Go to  Loop
    end if
    End of chain = row no
    If row no < L
    then
        For row no = row no after home  to   L
            If  code column (row no) = -1
            and pointer column (row no) = -2
                             (i.e. not in another chain)
            then
                Go to  Extend chain
            end if
        next row no
    end if
    If  row no before home >= 0
    then
        For row no = 0  to  row no before home
            If  code column (row no) = -1
            and pointer column (row no) = -2
            then
                Go to  Extend chain
            end if
        next row no
    end if
    Print  'Table full'
    Return                   (object not added)
Extend chain:
    Previous row no in chain = end of chain
    Pointer column (end of chain) = row no
    Pointer column (row no) = -1
Put object:
    Code column (row no) = object code
    Status column (row no) = object status
    X column (row no) = X
    Y column (row no) = Y
    Z column (row no) = Z
    Entry row no = row no
    Return                   (object successfully added)

BASIC:

    5000 R = W * L : ER = -1 : PR = -1 : BH = R - 1 : AH = R + 1
    5010 IF OC(R) >= 0 THEN 5040
    5020 IF OP(R) = -2 THEN OP(R) = -1
    5030 OC(R)=CD : OS(R)=ST : OX(R)=X : OY(Y)=Y : OZ(Z)=Z : ER=R : RETURN
    5040 IF OP(R) >= 0 THEN PR=R : R=OP(R) : GO TO 5010
    5050 EC=R
    5060 IF  R >= L THEN 5100
    5070 FOR  R = AH TO L
    5080 IF OC(R) = -1 AND OP(R) = -2 THEN 5150
    5090 NEXT R
    5100 IF  BH < 0 THEN 5140
    5110 FOR  R = 0 THEN BH
    5120 IF OC(R) = -1 AND OP(R) = -2 THEN 5150
    5130 NEXT R
    5140 PRINT 'TABLE FULL' : RETURN
    5150 OP(EC) = R : OP(R) = -1 : PR = EC : GO TO 5030

Listing 5. Subroutine: Delete object from

Used if an object is deposited at a location (X, Y, Z) from which
an identical object has been removed earlier. Depends on the sub-
routine 'Find object' (Listing 2) having been called first to set
the required input parameters.

    Inputs:  Entry row no, previous row no in chain, table.
    Outputs: Tables.

NB: There are four possibilities:
    (1) Entry is in home location with no chain (previous row no
        = -1, pointer column (entry row no) = -1).
    (2) Entry is in home location and there is a chain (previous
        row no = -1, pointer column (entry row no) >= 0 ).
    (3) Entry is not in home location but is at end of chain
        (previous row no >= 0, pointer column (entry row no) = -1 ).
    (4) Entry is not in home location nor at end of chain (previous
        row no >= 0, pointer column (entry row no) >= 0 ).

Pseudo-code:

    If previous row no in chain = -1
    then                    ( case (1) or (2) )
        If  pointer column (entry row no) = -1
        then                ( case (1) only 1 chain link
                              must remain in case (2) )
            Pointer column (entry row no) = -2
        end if
    else
        If  pointer column (entry row noO = -1
        then
            Pointer column (previous row no in chain) = -1
            Pointer column (entry row no) = -2
        end if
    end if
    Code column (entry row no) = -1    ( entry now free)
    Return

BASIC:

    5300 IF PR >= 0 THEN 5330
    5310 IF OP(R) = -1 THEN OP(R) = -2
    5320 OC(R) = -1 : RETURN
    5330 IF OP(R) = -1 THEN OP(PR) = -1 : OP(R) = -2
    5340 OC(R) = -1 : RETURN
```

I was wondering how many other articles that Graham wrote, on this particular subject. However, apparently, Graham Relf only wrote two articles for *Practical Computing*, on the subject of Mazes - from [The Forest - History](https://www.grelf.net/foresthistory.html), right at the bottom of the page:

> In 1982 I had two articles published in the now long-defunct British magazine "Practical Computing". The articles described the basic techniques I was using to generate terrain from mathematical functions rather than storing it in memory.





  [1]: https://github.com/greenonline/AdventureFunction/blob/main/xtras/images/Maze%20Movement%20-%20Page%201.png "Maze movement - page 1"
  [2]: https://github.com/greenonline/AdventureFunction/blob/main/xtras/images/Maze%20Movement%20-%20Page%202.png "Maze movement - page 2"
  [3]: https://github.com/greenonline/AdventureFunction/blob/main/xtras/images/Maze%20Movement%20-%20Page%203.png "Maze movement - page 3"





  
