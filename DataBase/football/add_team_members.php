<?php
include "config.php";
$p_id=$_REQUEST['id'];
$S_key=$_REQUEST['sk'];
$return_arr = array();
$row_array = array();

$sqlteam_info="SELECT * FROM `team_info` WHERE `Secret_key`='".$S_key."' LIMIT 1";
	$result_team_info=mysqli_query($conn, $sqlteam_info);

 if (mysqli_num_rows($result_team_info) > 0) 
{
 while($row_team_info = mysqli_fetch_assoc($result_team_info)) 
 {
	 $return_arr['id'] =$row_team_info["team_id"];
	
	$return_arr['s_key'] =$row_team_info['Secret_key'];
$sql = "INSERT INTO `team_members`(`pl_id`, `team_id`) VALUES ('".$p_id."',".$row_team_info["team_id"].")" ;
	
if (mysqli_query($conn, $sql))
{
	
}

   
  }  
  

}



else
{
  $return_arr['id'] ="error";
}


echo json_encode($return_arr);
mysqli_close($conn);	 
?>