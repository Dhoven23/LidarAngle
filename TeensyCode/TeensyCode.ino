/* Here are my globals.*/
float rot_X, // angle about X
      rot_Y,  // angle about Y
      distance;
      
int rad = 30;


void setup() {
  
  Serial.begin(9600);
  delay(100);

}

void loop() {

  
  /* call LidarAngle(int points[3], float rad) 
   * where points is the distance reading of each sensor. 
   * if the drill is pointing into the page, the assumed orientation is as follows. 
   * obviously this is mutable
   *
   *  (3) . . . (2)   (+x) < - - 0
   *    .       .                |
   *     .     .                 |
   *      .   .                  v
   *       (1)                  (+y)
   *  
   * rad is the radius about which the sensors are located, 
   * with points[0] forward reading from the sensor located in the 
   * <0,1,0> direction (+y) ex.
   */

   
   int points[3] = {120, 100, 100}; // test points, I used mm
   
   Serial.println(" \n\nNew data: --------------- ");
   
   LidarAngle(points,rad); //call the function like so
   
   Serial.print("Up/down angle = "),Serial.println(rot_X*(180/PI)); //angle is returned in rad.
   Serial.print("Left/right angle = "),Serial.println(rot_Y*(180/PI));
   Serial.print("Distance to wall: "),Serial.print(distance),Serial.println("mm");
   // blah blah...
   delay(10000);

}

void LidarAngle(int points[3], float rad){

  /* create vectors P and Q (P = points[0]->points[1], Q = points[0]->points[2]) */

  float cords[3][3] = {
    {
      0, rad, points[0]
    },{ 
      cos(PI/6)*rad, -sin(PI/6)*rad, points[1] 
    },{
      -cos(PI/6)*rad, -sin(PI/6)*rad, points[2]
    }
  };
  
    float P[3] = {cords[1][0]-cords[0][0], cords[1][1]-cords[0][1], cords[1][2]-cords[0][2]},
          Q[3] = {cords[2][0]-cords[0][0], cords[2][1]-cords[0][1], cords[2][2]-cords[0][2]};


  
  /* compute R, the cross product (PxQ), also the normal of the plane */
  
    float R[3] = {(P[1]*Q[2] - P[2]*Q[1]), (P[2]*Q[0] - P[0]*Q[2]), (P[0]*Q[1] - P[1]*Q[0])};

  /* Normalize R, and project onto X and Y axes */
  
    float norm_R = sqrt((R[0]*R[0] + R[1]*R[1] + R[2]*R[2]));
    float R_x = R[0]/norm_R;
    float R_y = R[1]/norm_R;
    
      rot_X = asin(R_x); // Globals are modified
      rot_Y = asin(R_y); // smae

  /* Finally compute the distance to the plane intersection, taken as 
   *  the normal intersection of the direction vector <0,0,1> with the plane defined by R
   */

   
      distance = (points[0] * cos(rot_Y) * cos(rot_X)) - rad*sin(rot_X);


/* print shiz, you can delete whatever you don't need access to */


  Serial.println("Plane intersections:");
  
    for(int i=0;i<3;i++){
        Serial.print("| ");
        for(int j=0;j<3;j++){
            Serial.print(cords[i][j]),Serial.print(' ');
        }
      Serial.println("|");
  }

  Serial.print("\nPlane normal vector:\nR = "),Serial.print(R[0]/norm_R);
  Serial.print("i +  "),Serial.print(R[1]/norm_R);
  Serial.print("j + "),Serial.print(R[2]/norm_R);
  Serial.println("k\n");
  
}
