<?php
include "config.php";
$id=$_REQUEST['id'];
$name_fx=$_REQUEST['name'];
$email_fx=$_REQUEST['email'];
$last_id;
if($id)
{
	
$sql = "SELECT * FROM `tblmember` WHERE `pl_id`=".$id;
}
else
{
$last_id =uniqid('pl_');
$sql = "INSERT INTO `tblmember`(`pl_id`,`fName`,`email`)  VALUES ('".$last_id."','".$name_fx."','".$email_fx."')";
}
$return_arr = array();
$row_array = array();

	//echo $sql;
	$result=mysqli_query($conn, $sql);
if ($result) 
{
	
  if($id)
  {
	  if (mysqli_num_rows($result) > 0) 
	  {
		 
		
		  while($row = mysqli_fetch_assoc($result)) {
	  //add more info for player to get 
         $last_id=$row['pl_id'];
		 $return_arr['id'] = $last_id;
		$return_arr['name'] =$row['fName'];
		$return_arr['email'] =$row['email'];
		$return_arr['type'] =$row['player_type'];
		 
            }
		  
	  }
	  else
	  {
		  $return_arr['id'] ="error";
	  }
	  
  }
  else{
	  

   $return_arr['id'] = $last_id;
   $return_arr['name'] = $name_fx;
   $return_arr['email'] = $email_fx;
   $return_arr['type'] ="new";
  }
 
} 
else 
{
  $return_arr['id'] ="error";
}

mysqli_close($conn);

/*else{
$return_arr['id'] ="error";	
}*/
echo json_encode($return_arr);
?>