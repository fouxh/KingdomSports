<?php
include "config.php";
$row_array = array();
$sql = "SELECT * FROM `match_making` LIMIT 2";
$result = mysqli_query($conn, $sql);

$row = mysqli_fetch_array($result, MYSQLI_NUM);

$row_array['red']= $row[1];
$row = mysqli_fetch_array($result, MYSQLI_NUM);

$row_array['blue']= $row[1];
// Free result set
mysqli_free_result($result);
$sqlMatch="INSERT INTO `match_info`(`red_team`, `blue_team`) VALUES (".$row_array['red'].",".$row_array['blue'].")";
mysqli_query($conn, $sqlMatch);
 $last_id = mysqli_insert_id($conn);
 $row_array['id']=$last_id;
$sqlDelet="DELETE FROM `match_making` WHERE `Team_id`=".$row_array['red']." OR `Team_id`=".$row_array['blue'];
mysqli_query($conn, $sqlDelet);
/*echo json_encode($row_array);
mysqli_close($con);*/
include "GetMatchId.php";

?>
