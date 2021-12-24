<?php
include "config.php";
$p_id=$_REQUEST['id'];
//$S_key=uniqid('sk_');
$S_key=$_REQUEST['sk'];
$return_arr = array();


 if(CheckValueDb('team_info', 'Secret_key="'.$S_key.'"') )
{
	 $sqlteam="SELECT * FROM `team_info` WHERE `Secret_key`='".$S_key."' LIMIT 1";
	$result=mysqli_query($conn, $sqlteam);

 if (mysqli_num_rows($result) > 0) 
{
 while($row = mysqli_fetch_assoc($result)) 
 {
	 $return_arr['id'] =$row["team_id"];
	
	$return_arr['s_key'] =$row['Secret_key'];

   
  }  

}
}
else{
	

$sql = "INSERT INTO `team_info`(`Owner_id`, `Secret_key`) VALUES ('".$p_id."','".$S_key."')" ;
	
if (mysqli_query($conn, $sql) && $S_key && $S_key!="" && $p_id && $p_id!="")
{
	 $last_id = mysqli_insert_id($conn);
	 $sqlteam="SELECT * FROM `team_info` WHERE `team_id`='".$last_id."' LIMIT 1";
	$result=mysqli_query($conn, $sqlteam);

 if (mysqli_num_rows($result) > 0) 
{
 while($row = mysqli_fetch_assoc($result)) 
 {
	 $return_arr['id'] =$row["team_id"];
	
	$return_arr['s_key'] =$row['Secret_key'];

   
  }  

}
}
else
{
  $return_arr['id'] ="error";
}
}
mysqli_close($conn);

echo json_encode($return_arr);	 
?>