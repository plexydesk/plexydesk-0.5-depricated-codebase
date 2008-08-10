<?php
					foreach ($navigation_items->result() as $row)
					{
					?>
					
						<li><a href="<?php echo base_url() . "blog/category/" . $row->blog_cat_code; ?>"><?php echo $row->blog_cat_code; ?></a></li>
<?php
					}
?>
