<?php
include "config.php";
$return_arr = array();
$row_array = array();
$queue = array();
$array = array();
$number_platers;
$player_id=$_GET['id'];
$map_name=$_GET['map'];
//$sql = "SELECT `id`, `queue`, `q_id` FROM `queues` WHERE `status`=0 ";
$sql = "SELECT `id`, `queue`, `q_id` FROM `queues` WHERE `map`='".$map_name."' ORDER BY `id` DESC  LIMIT 1";
$result = mysqli_query($conn, $sql);
$session_q;
//echo mysqli_num_rows($result);
if (mysqli_num_rows($result) > 0) {
  // Output the queue array as a JSON response
  while($row = mysqli_fetch_assoc($result)) {
	 
  $queue=$row['queue'];
  
  $session_q=$row['q_id'];
  
  
  }
 
 
} else {
  
     $return_arr['message']="No queue found in the database";
        empty($array);
        $array[0]=$player_id;
		$session_q=uniqid('q_');
		 $return_arr['players']=$array;
		$sql_q = "INSERT INTO queues (queue,q_id,map) VALUES ('" . json_encode($array) . "','".$session_q."','".$map_name."')";
		//echo $sql_q;
		mysqli_query($conn, $sql_q);
		$return_arr['session_id']=$session_q;
mysqli_close($conn);
echo json_encode($return_arr);
		die;
}
$array = json_decode($queue, true);

$index = array_search($player_id, $array);

$number_platers =count($array);
if ($index !== FALSE) {
  
  $return_arr['message']="Player found at index $index\n";
   $return_arr['players']=$array;
} else {
	if($number_platers<6)
	{
	//sql update array
	$array[$number_platers]=$player_id;
		
	 $return_arr['players']=$array;
	 if(count($array)==6)
	 {
		$return_arr['message']="reach to max Member"; 
		
	
	$sql_m="INSERT INTO `ready_match`(`queue`, `q_id`,`map`) VALUES ('" . json_encode($array) . "','".$session_q."','".$map_name."') ";
	mysqli_query($conn, $sql_m);
	$sql_q="DELETE FROM `queues` WHERE `q_id`='".$session_q."'";
	 
	
	 }
	 else
	 {
		$return_arr['message']="Add new Member"; 
	$sql_q="UPDATE `queues` SET `queue`='".json_encode($array)."' WHERE `q_id`='".$session_q."'";
	 }
	
	mysqli_query($conn, $sql_q);
	}
	else{
		
		$return_arr['message']="reach max player please create new quee and new session ";
		$return_arr['players']=array($player_id);
		empty($array);
		//$queue2[0]=$player_id;
		$session_q=uniqid('q_');
		$sql_q = "INSERT INTO queues (queue,q_id,map) VALUES ('" . json_encode(array($player_id)) . "','".$session_q."','".$map_name."')";
		//echo $sql_q;
		mysqli_query($conn, $sql_q);
	}
 
}
$return_arr['session_id']=$session_q;
mysqli_close($conn);
echo json_encode($return_arr);
?>

