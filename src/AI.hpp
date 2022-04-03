#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include"game_board.hpp"
#include"pieces.hpp"
#include"display_UI.hpp"

extern game_board main_bord;

class AI{
    
public:
    
    AI(){
        CPC = main_bord.GetPieceData(1).size();
    }
    void decision(); // for running other function

private:
    
    void attack(pieces&); // process piec for attack
    
    void defend(pieces&,const pieces&); // process piece for defend self
    
    void must_be_move(pieces&);// if are piece in equal chance to move
    
    void simulation(); // simulate battle and manage between three main function (defend, attack , must_be_nove)
    void OBJECT_CREATOR(); // creat object
    
    bool set_data_on_screen(int); // send final result to game board

    inline bool search_list(const char*); // search in equal movment in enemy
        
    inline int random_selector(int,int);// selet random way
    
    inline void SetMapValue(pieces*,const char*,int); // sort and put value in map
    /*  sort piece in map key:
        
            piece value:
            king pieces has: 100X
            normal pieces has: 10X

            movment value:
            defend data has: 100X
            attack data has: 1X >> for any score value has been increase 1 * 10^ (size of delete vector)-1
            Normal data has: 1X

            dangerous attack has:-1X;
            dangerous defend has:-1X;
    */
    std::map< int ,pieces*, std::less<int> > OrderedPiece;
    
    std::vector<const char*> Done;
    
    int CPC;// compeleted process counter
    
    std::vector<pieces> HumanMoveAnalyze;
    
    std::vector<pieces> ComputerMoveAnalyze;
};

#endif