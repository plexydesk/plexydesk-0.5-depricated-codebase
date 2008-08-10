		<div id="main">
			<div class="main-block">
<?php
				foreach ($entry_data->result() as $row)
				{
?>				<h1><?php echo $row->entry_title; ?></h1>
				<p>
					<span class="small-calender"><span class="year">2008</span><span class="date">23</span><span class="month">SEP</span></span>
					<?php echo $row->entry_intro; ?>
					
				</p>
				<p>
					<?php echo $row->entry_body; ?>
					
				</p>
				<p class="post-footer"><span class="category">Posted in : <a href="<?php echo base_url() . "blog/category/" . $row->entry_cat; ?>"><?php echo $row->entry_cat; ?></a></span><span class="author">Comments : (need help) &nbsp;&nbsp;  Post by : <a href="#"><?php echo $row->entry_author; ?></a></span></p>
<?php					
				}
?>					
				<h1>Comments</h1>
<?php
				foreach ($comment_data->result() as $row)
				{
?>				<p class="post-footer"><span class="category bold-italic"><span class="green-font"><?php echo $row->comment_by; ?></span> Said</span><span class="author"><?php echo $row->comment_added; ?></span></p>
				<p class="comment">
					<?php echo $row->comment_body; ?>
				</p>
<?php
				}
?>
				<h2>Add your comments</h2>
				<form>
					<ul class="form float-left">
						<li>
							<label>Name<em>required</em></label>
							<input type="text" size="60">
						</li>
						<li>
							<label>Email<em>required</em></label>
							<input type="text" size="40">
						</li>
						<li>
							<label>Description<em>required</em></label>
							<textarea>
							</textarea>
						</li>
						<li>
							<label>Verification<em>required</em></label>
							<input type="text" size="15">
						</li>
						<li>
							image
						</li>
						<li class="button">
							<input type="submit" value="Submit">
							<input type="reset" value="Reset">
						</li>
					</ul>
				</form>
			</div>
		</div>
