<?php

class Blog extends Controller {

	function Blog()
	{
		parent::Controller();
		//$this->load->model('Mdl_blog');
		
		//$this->load->scaffolding('tbl_blog_entries');
	}
	
	function index()
	{	
		$page_data['page_title'] = "Blog";
		
		$this->application->app_top($page_data);
		$this->display->head();
		
		// blog home main content
		$this->Mdl_blog->blog_home();
		// blog home side bar content
		$this->Mdl_blog->blog_home_side_bar();
		
		$this->display->footer();
	}
	/*	
	function enrty($post_id)
	{
		$page_data['page_title'] = "Blog";
		
		$this->application->app_top($page_data);
		$this->display->head();
		
		// display blog entry
		$this->Mdl_blog->blog_entry($post_id);
		
		// blog home side bar content
		$this->Mdl_blog->blog_home_side_bar();
		
		$this->display->footer();
	}
	
	function category($entry_cat_id)
	{
		$page_data['page_title'] = "Blog";
		
		$this->application->app_top($page_data);
		$this->display->head();
		
		// display blog entry
		$this->Mdl_blog->blog_category($entry_cat_id);
		
		// blog home side bar content
		$this->Mdl_blog->blog_home_side_bar();
		
		$this->display->footer();
	}
	*/
}
?>