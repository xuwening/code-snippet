<?php
for ($i = 0; $i <= 100; $i++) {
  printf("mprogress: %d%% %s\r", $i, str_repeat('#',$i) );
  usleep(1000 * 100);
}
echo "\n", "Done.\n";
?>
