<?php
include "config.php";
$p_id=$_REQUEST['PID'];

//GetRewards.php?PID=ghghg
$sql = "SELECT * FROM `player_info`  WHERE `PlayerID`='".$p_id."'";

if ($result = mysqli_query($conn, $sql)) {

if (mysqli_num_rows($result) > 0) {
  // output data of each row
  while($row = mysqli_fetch_assoc($result)) {
	  echo json_encode($row);
   // echo "id: " . $row["id"]. " - path: " . $row["path"]. " " . $row["Status"]. "<br>";
  }
} else {
 // echo "0 results";
$arr_no_value = array("id"=>0);

echo json_encode($arr_no_value);
}

}
mysqli_close($conn);
?>