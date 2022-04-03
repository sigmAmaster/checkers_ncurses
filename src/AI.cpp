#include"AI.hpp"
#include<chrono>
#include<random>
#include<cmath>
#include<iterator>

void AI::decision(){
   std::vector<int> collect_key;
   
   bool set_data_res = false;   

   auto point = OrderedPiece.rbegin();

   int loop = 0;
   
   simulation();

   // after call simulation function 
   // we need to get decision
   while ( set_data_res != true){
   
      // detect key from defend function
      if (point->first >= 1000 )
      set_data_res = set_data_on_screen( point->first );
      // detect key from attck function
      else if(point->first%2 == 0)
      set_data_res = set_data_on_screen( point->first );
      
      // search for good key 
      else{
         
         // collect good key
         for (auto &counter:OrderedPiece)
            if (counter.first > 0)
               collect_key.push_back(counter.first);
         
         // if you collect any good key
         if ( !collect_key.empty() ){

            int randnum = random_selector(0, ( collect_key.size()-1 ) );
            set_data_res = set_data_on_screen( collect_key[ randnum ] );
         }
         
         // if we have only bad key we must be do that :(
         else
         set_data_res = set_data_on_screen( point->first );
      }
      
      //try another piece in list
      point++;
      
      // we cant move piece
      if (OrderedPiece.size() == loop )
         break;   
      
      else
      loop++;      

   }   
   return;
}

bool AI::set_data_on_screen (int KEY){
   
   const char* enemy_piece=nullptr;
   const char* PieceLocal=nullptr;
   const char* MoveBlock=nullptr;
   
   // if we haven't any delet data 
   if ( !OrderedPiece[KEY]->movement.empty() ){
         
      if (OrderedPiece[KEY]->deleted_pieces.empty()){
            
         PieceLocal=OrderedPiece[KEY]->piece_shape;
         
         MoveBlock = *OrderedPiece[KEY]->movement.begin();

         main_bord.delete_data(enemy_piece,PieceLocal,MoveBlock,false);
         return true;
         
      }

      else{
         
         if ( OrderedPiece[KEY]->movement[0] != nullptr){

            auto data_sender = [&](std::pair<const char*,const char*> delete_data){
            
               PieceLocal = OrderedPiece[KEY]->piece_shape;
               MoveBlock = delete_data.first;
               enemy_piece = delete_data.second;

               main_bord.delete_data(enemy_piece,PieceLocal,MoveBlock,false);
               OrderedPiece[KEY]->piece_shape = MoveBlock;
            }; 
   
            for (auto &Data_D:OrderedPiece[KEY]->deleted_pieces)
               data_sender(Data_D);
            return true;
         }
      }
   } 
   
   // this piece movment is incorret data back and select other one 
   return false;
}
void AI::OBJECT_CREATOR(){
 
 const std::vector<char *>& ComputerP = main_bord.GetPieceData(1); 
 const std::vector<char *>& HumanP = main_bord.GetPieceData(2);
 
 auto OBJect_creation =[&](const char * postion,bool turn){
     // make *OBJect for every piece and calculate all movement

   pieces ANALYZE; 
    
    ANALYZE.piece_shape = postion;

    ANALYZE.pieceY = main_bord.find_location_pice(postion,true);
    ANALYZE.pieceX = main_bord.find_location_pice(postion,false);
    
    if(turn == true)   
      HumanMoveAnalyze.push_back(ANALYZE);
    else
      ComputerMoveAnalyze.push_back(ANALYZE);
 };
 
 for (const auto &i:HumanP) 
    OBJect_creation(i,true);

 for (const auto &i:ComputerP)
   OBJect_creation(i,false);

 
 
 // call a two groups of threads for process all actions for *OBJect in list
 // send piece for movement process
 auto FUNC = [](pieces& OBJECT){OBJECT.movment_calculate();}; 

 // human process group
 std::for_each( HumanMoveAnalyze.begin(), HumanMoveAnalyze.end(),FUNC);

 // computer process group 
 std::for_each(ComputerMoveAnalyze.begin(),ComputerMoveAnalyze.end(),FUNC);

}
//
void AI::simulation(){

 OBJECT_CREATOR();
 
   for (auto &solider:ComputerMoveAnalyze){ // search for good attack 
      if (!solider.deleted_pieces.empty()){
         // we wait for finish process
         attack(solider);
      }
   }
   
 // after process all attack chance we find any piece in danger  
   for(auto &find_danger:HumanMoveAnalyze){

      if (!find_danger.deleted_pieces.empty()){// if you find in danger piece        
       
       std::vector<pieces*> inDangerPiece;

         for (auto &ComPiece:ComputerMoveAnalyze){
            // search for all *OBJect for find equality in delete list
            // all in danger *OBJect send to defend
           
            for (auto &DeletList:find_danger.deleted_pieces){
            
               if (ComPiece.piece_shape == DeletList.second){
                  inDangerPiece.push_back(&ComPiece);
               }
            }
         }
                    
       for (auto &SendOBJect:inDangerPiece){ defend(*SendOBJect,find_danger); }
      }
   }
   
   for (auto &ForcedMovement:ComputerMoveAnalyze){
      
      // if we dont any piece yet
      if (OrderedPiece.empty())
         must_be_move(ForcedMovement);
       
      else{
         bool is_found=false;
         
         // search for equal piece , we dont want process again
         for (auto &Compeleted:OrderedPiece)    
            if ( Compeleted.second == &ForcedMovement )
            is_found=true;
       
         if (is_found == false){
            must_be_move(ForcedMovement);
         }
      }
       
   }
   
   
  return;
}

//////////////////////////// process defened for any piece
void AI::defend(pieces& IN_DANGER,const pieces& ENEMY){
   // IN DANGER IS COMPUTER PIECE IN DANGER POSTION

   std::vector<const char*> dangerous_way;
   const char* correct_way = nullptr;
   
      for (auto way:IN_DANGER.movement){
         bool res = search_list(way);

         if (res == false)
            dangerous_way.push_back(way);
         
      }
     
   if ( dangerous_way.empty() && !IN_DANGER.movement.empty() ){
      // if we dont have dangerous way so select random way for running
      
      int rand_correct_way = random_selector(0, ( IN_DANGER.movement.size()-1 ) );
      correct_way = IN_DANGER.movement[rand_correct_way]; 
      
      SetMapValue(&IN_DANGER,correct_way,1);
   }

   else if( !IN_DANGER.movement.empty() ){
      // so we have dangerous way and we need be careful

      for (auto &select_way:IN_DANGER.movement){
         
         //search all movment list, if they doesn't find in dangerous way
         // this is first way for moving

        for(auto &this_way:dangerous_way){

            if (select_way !=  this_way){
               correct_way = select_way;   
               SetMapValue(&IN_DANGER,correct_way,1);
            }
         }
      }
   }   
    
   else{
    // if all movment way are close we find hero piece to hit or defend our piece
      
      for (auto &Hero:ComputerMoveAnalyze){
        
         // find in piece for hit enemy piece
         for ( auto &find:Hero.deleted_pieces){
           
            if (find.second == ENEMY.piece_shape ){             
               
               bool answer = search_list(find.first); 
               
               if ( answer == true){
                  correct_way = find.first;
                     SetMapValue(&IN_DANGER,correct_way,1);
               }
            }
         }
         
      }  
   }
   
   /* 
      if we dont find any hero and we have way for move 
      but it is can be dangerous, we had to move 
      becuase if we dont have any other piece for move we need
      to move this piece but on last of map for move 
   */  
  
  if (correct_way == nullptr && !IN_DANGER.movement.empty() ){
     
     int index = random_selector( 0, IN_DANGER.movement.size() );
      correct_way = IN_DANGER.movement[index];

     SetMapValue(&IN_DANGER,correct_way,4);
         return;
  }
  
   return;
}

/////////////////////////////// make random number for select
inline int AI::random_selector (int beginPoint,int endPoint){
/*
   auto now = std::chrono::high_resolution_clock::now();

   std::seed_seq seed{
      std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count()
   };
*/
   std::random_device rd;
  
   std::mt19937_64 eng(rd());
      
   std::uniform_int_distribution<unsigned int>distro(beginPoint,endPoint);
      
   return distro(eng); 
}

////////////////////////////////// process attack for any piece
void AI::attack(pieces& ATTACKER){
   
   std::vector<const char*> dangerous_way;
   const char* correct_way = nullptr;
   
   // search for all human piece for equality movment
   for (auto &EnemyOBJect:HumanMoveAnalyze){
      for (auto &EnemyWay:EnemyOBJect.movement){
       
       auto way_pointer = ATTACKER.deleted_pieces.find(EnemyWay);
         
         //if that way is equal to enemy movment put in dangerous way
         if (way_pointer != ATTACKER.deleted_pieces.end())
            dangerous_way.push_back(EnemyWay);
      }   
   }
   
   // lambada function if we dont have any dangerous way
   auto zero_dangerous = [&](){
      int counter = 0;
         int rand_correct_way = random_selector(0,( ATTACKER.deleted_pieces.size()-1 ) );

         for (auto loop=ATTACKER.deleted_pieces.begin();loop != ATTACKER.deleted_pieces.end();loop++){
         
            if (counter == rand_correct_way){
               correct_way = loop->first;
                  SetMapValue(&ATTACKER,correct_way,2);
            }

            else
            counter++;
         }  
   };

   // if size bigger than one attacker can risk for moving because can delete more enemy piece  
   if (ATTACKER.deleted_pieces.size() > 1){
      // if we dont have any dangerous way select random
      
      
      if (dangerous_way.empty())   
         zero_dangerous();
      
      else{
         correct_way = ATTACKER.deleted_pieces.begin()->first;
         SetMapValue(&ATTACKER,correct_way,2);
      }
   }
   else{
      // check delete list
       if (dangerous_way.empty())   
         zero_dangerous();
      
      else{
         correct_way = ATTACKER.deleted_pieces.begin()->first;
         SetMapValue(&ATTACKER,correct_way,4);      
      }
   }

   return;      
}

void AI::must_be_move (pieces& ThisPiece){

   const char *correct_way = nullptr;
   std::vector<const char*> BadWay;

   // if you piece can move
   if (  !ThisPiece.movement.empty() ){
      
      // search for dangerous movment
      for (auto way:ThisPiece.movement){
         bool result = search_list(way);

         if (result == false)
            BadWay.push_back(way);      
      }

      // if we are safe in all posion
      if (BadWay.empty()){
         
         int randomNUM = random_selector(0,( ThisPiece.movement.size()-1 ) );
         correct_way = ThisPiece.movement[randomNUM];
      
         SetMapValue(&ThisPiece,correct_way,3);
      }
      
      else{
         
         // search for better move?
         for (auto &way:ThisPiece.movement){
         
            auto bad_way_finder = std::find(BadWay.begin(),BadWay.end(),way);// your way is safe?

            if (bad_way_finder != BadWay.end())
               continue;
            
            else{
               correct_way = way;
               SetMapValue(&ThisPiece,correct_way,3);
            }
         
         }
               
         if (correct_way== nullptr &&  !ThisPiece.movement.empty()){
            
            int randomNUM = random_selector(0, ( ThisPiece.movement.size()-1 ) );
            correct_way = ThisPiece.movement[randomNUM];
            SetMapValue( &ThisPiece, correct_way, 4);
         }
         
      }
      
   }

 
   return;
}

inline bool AI::search_list(const char* way){
   
   for (auto &enemyPiece:HumanMoveAnalyze){

     /* analyze all human piece for movment, computer movment cant,t equal 
         with human piece move, if we have piece for move, if not we had 
         to do that
      */
   
       auto identfy_way = std::find(enemyPiece.movement.begin(),enemyPiece.movement.end(),way);

         if ( identfy_way != enemyPiece.movement.end() )
            return false ;
   }

  return true;
}

inline void AI::SetMapValue (pieces* Piece,const char* correct_way,int callBY){

   int Function_Value = 0;

   switch (callBY){
   
   case 1: Function_Value = 100; break;
   case 2: { Function_Value = pow( 10, ( Piece->deleted_pieces.size() -1 ) ) * 10; }; break;
   
   case 3: Function_Value = 1; break;
   case 4: Function_Value = -1; break;
   
   default: 
   exit(EXIT_FAILURE);break;
   }

   if(CPC != 0 ){
      *Piece->movement.begin() = correct_way;
      
      if (Function_Value != -1){

         if ( *Piece->piece_shape == 'C' )
            OrderedPiece[ ( CPC * 100 * Function_Value ) ] = Piece;
         
         else
            OrderedPiece[ ( CPC * 10 * Function_Value ) ] = Piece;
      }

      else{
         const char * BadWay = nullptr;
         
         int RandBadWay = random_selector(0, ( Piece->movement.size()-1 ) );
         BadWay = Piece->movement[RandBadWay]; 

         *Piece->movement.begin() = BadWay;
         OrderedPiece[ ( CPC * Function_Value ) ] = Piece;      
      }

      CPC--;
      return;
   }

   else // if CPC is zero that means our process is finished
   return;
}