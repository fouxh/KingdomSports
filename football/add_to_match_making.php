<?php
include "config.php";
$id_team=$_REQUEST['id_team'];
$return_arr = array();
if(CheckValueDb("match_making", "Team_id=".$id_team))
{
$return_arr['id']="Ok";		
}
else
{
$sqlteam="INSERT INTO `match_making`(`Team_id`) VALUES (".$id_team.")";
$result=mysqli_query($conn, $sqlteam);
$last_id = mysqli_insert_id($conn);
if($last_id>0)
{
$return_arr['id']="Ok";	
}
else
{
$return_arr['id']="Error";		
}

}

 
echo json_encode($return_arr);
mysqli_close($con);

?>
