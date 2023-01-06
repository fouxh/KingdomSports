<?php
include "config.php";
// Query the table to get the queue array
//http://localhost/atlants/get_queue_by_id.php?q_id=q_63af558c5acb1&p_id=ahmed1
$queue_id=$_GET['q_id'];
$player_id=$_GET['p_id'];
$return_arr = array();
$queue = array();
$sql = "SELECT * FROM ready_match where q_id='".$queue_id."'";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
  // Output the queue array as a JSON response
  while($row = mysqli_fetch_assoc($result)) {
	  // echo json_encode($row['queue']);
  $queue=$row['queue'];
$return_arr['players']=$queue;
$return_arr['url']=$row['url'];
$array = json_decode($queue, true);
$index = array_search($player_id, $array);
	if($index<2)
	{
	$return_arr['team']	="red";
	}
	else
	{
	$return_arr['team']	="blue";	
	}



	
   //array_push($return_arr,$row['queue']);
  }
 
 
} 
else 
{
 $return_arr['url']="null";
}

// Close connection
mysqli_close($conn);
echo  json_encode($return_arr);
?>


