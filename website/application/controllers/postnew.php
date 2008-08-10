<?php

class Postnew extends Controller {

	function Postnew()
	{
		parent::Controller();
		$this->load->model('Mdl_blog');
		$this->load->model('admin');
	}
	
	function index()
	{	
		$page_data['page_title'] = "Blog";
		
		$this->application->app_top($page_data);
		$this->display->head();
		
		// entry posting form
		$this->admin->post_entry();

		$this->Mdl_blog->blog_home_side_bar();
		
		$this->display->footer();
	}
	function new_category()
	{
		$page_data['page_title'] = "Blog";
		
		$this->application->app_top($page_data);
		$this->display->head();
		
		// blog category form
		$this->admin->new_cat();

		$this->Mdl_blog->blog_home_side_bar();
		
		$this->display->footer();		
	}
}
?>