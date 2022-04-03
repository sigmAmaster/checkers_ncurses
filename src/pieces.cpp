#include"pieces.hpp"


void pieces::movment_calculate(){

   void (pieces::*ptr)( void ) = &pieces::movment_calculate;

   if ( *piece_shape == '@'){// detect type of piece
         
      // if piece on the corner becuase we dont want calculate out side the box
      // saftey condition for process x lines 
      if ( pieceX == 0 || pieceX == ( max_bord_x-1 ) /* 19 */ ){  
            
         if (pieceX == 0 ){// if piece on the left corner
            
            // detect one and two Further block;
            // right means piece go to right because left is close and goes out of board scope
         
            right_up = &play_ground[pieceY-1][pieceX+1]; 
            right_up_plus = &play_ground[pieceY-2][pieceX+2];
            
            loop_breaker( 1, ptr );
         }
            
         else{ 

            // that means piece is on right corner so that's like left corner but Reverse it
            left_up = &play_ground[pieceY-1][pieceX-1]; 
            left_up_plus = &play_ground[pieceY-2][pieceX-2];
                        
            loop_breaker( 2, ptr );
         }    
      }
       
      // piece is not in the corner
      else{

         right_up = &play_ground[pieceY-1][pieceX+1];
         left_up = &play_ground[pieceY-1][pieceX-1];

         /* if piece is near to corner beacuse we don't 
         want calulate two block further. two block further is out of scope 
         and it can be fill with garbage value
         */  

         // saftey check for y , the plus movment can go out of scope 
         if ( pieceY > 1 /* 18 */){
            
            if (pieceX == 1 || pieceX == ( max_bord_x-2 ) /* 18 */ ){
            
               if (pieceX == 1){
                  right_up_plus = &play_ground[pieceY-2][pieceX+2];
                  left_up_plus = nullptr;
               }
               else{
                  left_up_plus = &play_ground[pieceY-2][pieceX-2];
                  right_up_plus = nullptr;
               }
            }
        
            else{

               right_up_plus = &play_ground[pieceY-2][pieceX+2];
               left_up_plus = &play_ground[pieceY-2][pieceX-2];            
            }
         }

         loop_breaker( 1 , ptr );
         
         loop_breaker( 2 , ptr);
      }
   }
     
   else if( *piece_shape == '*' ){
    // for computer pieces  like up but direction are reverse
      
      if ( pieceX == 0 || pieceX == ( max_bord_x-1 ) /* 19 */ ){  
            
         if (pieceX == 0 ){
       
             right_down = &play_ground[pieceY+1][pieceX+1]; 
             right_down_plus = &play_ground[pieceY+2][pieceX+2];
            
            loop_breaker( 3 , ptr );
         }

         else{ 
            left_down = &play_ground[pieceY+1][pieceX-1]; 
            left_down_plus = &play_ground[pieceY+2][pieceX-2];
             
            loop_breaker( 4 , ptr );
       }  
      }
      
      else{
         right_down = &play_ground[pieceY+1][pieceX+1];
         left_down = &play_ground[pieceY+1][pieceX-1];

         if ( pieceY > ( max_bord_y-2 ) /* 18 */){
            
            if (pieceX == 1 || pieceX == ( max_bord_x-2 ) /* 18 */ ){
                  
               if (pieceX == 1){
               
                  right_down_plus = &play_ground[pieceY+2][pieceX+2];
                  left_down_plus = nullptr;
               }
               
               else{
                  left_down_plus = &play_ground[pieceY+2][pieceX-2];
                  right_down_plus = nullptr;
               }
            }
         }


         else{
            right_down_plus = &play_ground[pieceY+2][pieceX+2];
            left_down_plus = &play_ground[pieceY+2][pieceX-2];            
         }


         loop_breaker( 3 , ptr );         
         loop_breaker( 4 , ptr );
      }
   }

   else  
   king_movement();         
}
void pieces::data_filler( block_t wayOne, block_t wayTwo){

   char enemy_p_1 ='\0', enemy_p_2 ='\0';
   // if your human your enemy piece is equal to :
   if ( *piece_shape == 'H' || *piece_shape == '@'){
      enemy_p_1 = '*';
      enemy_p_2 = 'C';   
   }
   // if not your enemy is equal to:
   else{
      enemy_p_1 = '@';
      enemy_p_2 = 'H';
   }

   if( (*wayOne == enemy_p_1 || *wayOne == enemy_p_2) && wayTwo != nullptr){   // detect one block further if there is enemy piece
      
      // save ram address for two block further and get permission to move
      // so  two block further is empty and you can move and delet enemy piece too
      // saftey check for out of scope
      if ( *wayTwo == '#' && wayTwo != piece_shape ){

         deleted_pieces[wayTwo] = wayOne;
         movement.push_back(wayTwo );
         
         far_switch = true;
         movment_chain[wayTwo] = &play_ground[pieceY][pieceX];

         pieceY = main_bord.find_location_pice(wayTwo,true);
         pieceX = main_bord.find_location_pice(wayTwo,false);
         
      }     
      /* so one block further has enemy piece but if enemy piece is on
         two block further you cant move anymore
      */   
         
   }
         
   else if( (*wayOne == '#') && ( piece_shape == &play_ground[pieceY][pieceX]) ){ 
      movement.push_back(wayOne);}
         
   else far_switch = false;  

   return;
}

    inline void pieces::loop_breaker(int switcher, void (pieces::*that_func)( void )){

      switch (switcher){
      
      case 1 : { data_filler(right_up, right_up_plus); if( far_switch == false) return; 
                  far_switch = false; pieces*that_func( void );
               };break;
      
      case 2 : { data_filler(left_up, left_up_plus); if( far_switch == false) return;
                  far_switch = false; pieces*that_func( void );
               };break;
      
      case 3 : { data_filler(right_down, right_down_plus); if( far_switch == false) return;
                 far_switch = false; pieces*that_func( void );  
               };break;

      case 4 : { data_filler(left_down, left_down_plus); if( far_switch == false) return;
                 far_switch = false; pieces*that_func( void );
               };break;
      
      default: exit(EXIT_FAILURE);break;
      }
}
void pieces::king_movement(){
         
   void (pieces::*ptr)(void) = &pieces::king_movement;

      // if piece on the corner becuase we dont want calculate out side the box
      if ( pieceX == 0 || pieceX == ( max_bord_x-1 ) /* 19 */ ){  
            
         if (pieceX == 0 ){// if piece on the left corner
            
            // detect one and two Further block;
            // right means piece go to right because left is close and goes out of board scope
         
            right_up = &play_ground[pieceY-1][pieceX+1]; 
            right_up_plus = &play_ground[pieceY-2][pieceX+2];
            
            loop_breaker( 1, ptr );
         }
            
         else{ 
            // that means piece is on right corner so that's like left corner but Reverse it
            left_up = &play_ground[pieceY-1][pieceX-1]; 
            left_up_plus = &play_ground[pieceY-2][pieceX-2];

            loop_breaker( 2 , ptr );                        
         }    
      }
       
      // piece is not in the corner
      else{
         right_up = &play_ground[pieceY-1][pieceX+1];
         left_up = &play_ground[pieceY-1][pieceX-1];

         /* if piece is near to corner beacuse we don't 
         want calulate two block further. two block further is out of scope 
         and it can be fill with garbage value
         */  
            
         if (pieceX == 1 || pieceX == ( max_bord_x-2 ) /* 18 */ ){
            
            if (pieceX == 1)
            right_up_plus = &play_ground[pieceY-2][pieceX+2];
            else
            left_up_plus = &play_ground[pieceY-2][pieceX-2];
         }

         else{
            right_up_plus = &play_ground[pieceY-2][pieceX+2];
            left_up_plus = &play_ground[pieceY-2][pieceX-2];            
         }
         
         loop_breaker( 1 , ptr );
         loop_breaker( 2 , ptr );
      }
        
      if ( pieceX == 0 || pieceX == ( max_bord_x-1 ) /* 19 */ ){  
            
         if (pieceX == 0 ){
       
            right_down = &play_ground[pieceY+1][pieceX+1]; 
            right_down_plus = &play_ground[pieceY+2][pieceX+2];
            
            loop_breaker( 3 , ptr );
         }

         else{ 
            
            left_down = &play_ground[pieceY+1][pieceX-1]; 
            left_down_plus = &play_ground[pieceY+2][pieceX-2];
             
            loop_breaker( 4 , ptr );
         }  
      }
   
      else{
         right_down = &play_ground[pieceY+1][pieceX+1];
         left_down = &play_ground[pieceY+1][pieceX-1];

         if (pieceX == 1 || pieceX == ( max_bord_x-2 ) /* 18 */ ){
            
            if (pieceX == 1)
            right_down_plus = &play_ground[pieceY+2][pieceX+2];
            else
            left_down_plus = &play_ground[pieceY+2][pieceX-2];
         }

         else{
            right_down_plus = &play_ground[pieceY+2][pieceX+2];
            left_down_plus = &play_ground[pieceY+2][pieceX-2];            
         }
         
         loop_breaker( 3 , ptr );
         loop_breaker( 4 , ptr );
      }
   
      if (far_switch == true)            
      king_movement();
            
      else
      return;
}
