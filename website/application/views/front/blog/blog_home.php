		<div id="main">
			<div class="main-block">
<?php
				foreach ($latest_entries->result() as $row)
				{
?>				<h1><a href="<?php echo base_url() . "blog/enrty/" . $row->entry_id; ?>"><?php echo $row->entry_title; ?></a></h1>
				<p>
					<span class="small-calender"><span class="year">2008</span><span class="date">23</span><span class="month">SEP</span></span>
					<?php echo substr($row->entry_intro, 0, 500) . "...."; ?>
					
				</p>
				<p class="post-footer"><span class="category">Posted in : <a href="<?php echo base_url() . "blog/category/" .$row->entry_cat; ?>"><?php echo $row->entry_cat; ?></a></span><span class="author">Comments : (11) &nbsp;&nbsp;  Post by : <a href="#"><?php echo $row->entry_author; ?></a></span></p>
<?php
				}
?>
			</div>
		</div>
