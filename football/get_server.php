<?php
include "config.php";
$match_id=$_REQUEST['match_id'];
$team_id=$_REQUEST['team_id'];
if($match_id)
{
	
$sql = "SELECT `id`, `path`, `Status`,`match_id` FROM `url_service` WHERE `match_id`=".$match_id." OR `Status`=0  LIMIT 1";		
}
else
{
$sql = "SELECT `id`, `path`, `Status`,`match_id` FROM `url_service` WHERE `Status`=0 LIMIT 1";	
	
}



if ($result = mysqli_query($conn, $sql)) {
 // echo "New record created successfully";


if (mysqli_num_rows($result) > 0) {
  // output data of each row
  if($match_id)
  {
$sql_match = "UPDATE `death_match` SET `status`=1 WHERE `id_d_m`=".$match_id;
mysqli_query($conn, $sql_match);
$sql_team_match = "UPDATE `team_matchmaking` SET `match_id`=".$match_id." WHERE `team_id`=".$team_id;
mysqli_query($conn, $sql_team_match);		
 
  }
  while($row = mysqli_fetch_assoc($result)) {
	  echo json_encode($row);
   // echo "id: " . $row["id"]. " - path: " . $row["path"]. " " . $row["Status"]. "<br>";
  }
}
 else 
 {
 
// $arr_no_value = array("path"=>"null");
include "RefrechServer.php";
//echo json_encode($arr_no_value);
}

} 
else 
{

}
mysqli_close($conn);

?>