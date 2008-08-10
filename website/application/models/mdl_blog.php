<?php

	class Mdl_blog extends Model 
	{	
	    function Mdl_blog()
	    {
	        parent::Model();
	    }
		function blog_home()
		{
			// preparing the latest blog entries auery
			$this->db->select('entry_id, entry_cat, entry_title, entry_intro, entry_author, entry_added');
			$this->db->where('entry_status', 1);
			$this->db->limit(5);
			$this->db->order_by("entry_added", "desc");
			// writing to the blog navigation view
			$entries['latest_entries'] = $this->db->get('tbl_blog_entries');
			
			$this->load->view('front/blog/blog_home', $entries);
		}
		
		function blog_entry($post_id)
		{
			// preparing auery for retriving the entry data
			$this->db->select('entry_cat, entry_title, entry_intro, entry_body, entry_author, entry_added');
			$this->db->where('entry_id', $post_id);
			
			$blog_entry['entry_data'] = $this->db->get('tbl_blog_entries');
			
			// preparing query for retriving the coment related to the entry
			$this->db->select('comment_body, comment_by, comment_added');
			$this->db->where('entry_id', $post_id);
			$this->db->where('comment_status', 1);
			$this->db->order_by("comment_added", "desc");
			
			//$blog_entry['number_of_comments'] = $this->db->count_all_results('tbl_comments');
			$blog_entry['comment_data'] = $this->db->get('tbl_comments');
			
			// writing to the blog entry view
			$this->load->view('front/blog/blog_entry', $blog_entry);
		}
		
		function blog_category($entry_cat_id)
		{
			// preparing the latest blog entries auery
			$this->db->select('entry_id, entry_cat, entry_title, entry_intro, entry_author, entry_added');
			$this->db->where('entry_status', 1);
			$this->db->where('entry_cat', $entry_cat_id);
			$this->db->order_by("entry_added", "desc");
			
			$entries['latest_entries'] = $this->db->get('tbl_blog_entries');
			// writing to the blog home view (category  view and blog home views are same)
			$this->load->view('front/blog/blog_home', $entries);
		}
		
		function blog_home_side_bar()
		{	
			// preparing the blog category query
			$this->db->select('blog_cat_code, blog_cat_title');
			$this->db->where('blog_cat_status', 1);
			$this->db->order_by("blog_cat_order");
			// writing to the blog navigation view
			$blog_nav['navigation_items'] = $this->db->get('tbl_blog_categories');
			// include blog navigation in to the side bar
			$blog_side_bar = $this->load->view('front/blog/blog_navigation', $blog_nav, TRUE);
			
			// including the vertical image menu in to the side bar
			$blog_side_bar = $this->load->view('front/common/vertical_image_menu', '', TRUE);
			
			// some resent updates
			$blog_side_bar = $this->load->view('front/common/updates', '', TRUE);
			
			// loading the side bar
			$this->load->view('front/blog/blog_home_side_bar', $blog_side_bar);
			
		}
	}
	
?>