# blackjack_cardcounting

- Explanation
Player 1 calculates and operates the probability, and Player 2 operates according to the same rules as the dealer.
Compare the win rates of Player1 and Player2.


- 규칙
The player proceeds assuming the unseen dealer's card is 10.(There is a high probability that at least one of the two cards received initially contains 10, J, Q, K, and A.)
Player 2 does Stand if it is 17 or higher, and HIT if it is less than 17.
Player 1 hits and stands based on probability.(Calculate the probability using which card has already been played.)
The initial funds is 100,000 won.
The player bets 100 won at the start of the game.


- card counting method
  1. Add player 1's 2 cards, player 2's 2 cards and the dealer's 2nd card to the card counting.
  2. Player 1 decides to hit and stand.
    When player 1 hits, the received card is added to the card counting.
  3. When Player 1 finishes, Player 2 and the dealer proceed in order.
    3-1. Add each received card to card counting.
    3-2. Calculates the probability of busting when the dealer hits.
    3-3. Repeat player 1's action (2) through probability.
    *When shuffling using 80% of the cards, the card counting is reset to 0.
    
    
- A process
  1. You can use A by selecting 1 or 11.
  2. In order to avoid bust when calculating the card total, A used 11 at most once.
  3. It can be divided into a case where all A is used as 1 and a case where A is used as 11 once.
    
    
- Player 1 action (probability application method, A processing method)
  1. If there is 21 among the minimum and maximum values of player 1's card total, he must stand.
  2. If the maximum value is less than 10, the hit proceeds.
  3. If not 1,2, choose a hit and stand with the dealer's total cards.
    a. When the dealer hits
      a-1. If there is a high probability of bust
        stand
      a-2. If there is a high probability that a bust will not occur
        a-2-1. If its maximum value is bust, the minimum value is used.
                  If player1 minimum value is greater than or equal to the dealer's card total, stand.
                  If player1 minimum value is less than the dealer's total cards
                      When you hit with the minimum value, if the bust probability is more than 60%, stand
                      If not, hit.
        a-2-2. If player1 max is not bust
                  If player1 maximum value is greater than or equal to the dealer's total cards, stand.
                      When the maximum value is less than the dealer's total cards
                          When hit with the minimum value, if the bust probability is more than 60%, stand.
                          If not, hit. 
    b.When the dealer stands
        It works the same as a-2-2.

