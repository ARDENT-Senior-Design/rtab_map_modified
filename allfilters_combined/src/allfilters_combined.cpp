//***                   ARDENT                        ***
//***          Peception & Embedded System            ***
//***          Californi State University, Northridge ***
//***          2018-2019                              ***
//***	       LAST EDIT: FEBURARY 17, SPRING 2019    ***
//***						      ***
//***Discription: All the filters are combined in this***
//***standalone cpp. Includes Voxel grid filter to    ***
//***downsample with leaf size of 0.03 & Passthrough  ***
//***filter to get rid of all the data points after 4 ***
//***meters away in z direction			      ***


#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

//voxel grid library
#include <pcl/filters/voxel_grid.h>

//passthorugh library
#include <pcl/filters/passthrough.h>


ros::Publisher pub;

void cloud_cb(const pcl::PCLPointCloud2ConstPtr& input)
{

    //pcl::PCLPointCloud2 cloud_filtered;
    pcl::PCLPointCloud2::Ptr cloud_filtered (new pcl::PCLPointCloud2);
    pcl::PCLPointCloud2::Ptr cloud_pass(new pcl::PCLPointCloud2);

    pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
    pcl::PassThrough<pcl::PCLPointCloud2> pass_through_filter;

      //pass through filter
    pass_through_filter.setInputCloud (input);
    pass_through_filter.setFilterFieldName("z");
    pass_through_filter.setFilterLimits(0.0, 4.0);

    pass_through_filter.filter (*cloud_pass);
    //voxel grid filter
    sor.setInputCloud(cloud_pass);
    sor.setLeafSize(0.03, 0.03, 0.03);
    sor.filter(*cloud_filtered);
    // Publish the data
    pub.publish(*cloud_filtered);
}

int main (int argc, char** argv){
    // Initialize ROS
    ros::init (argc, argv, "allcombined");
    ros::NodeHandle nh;

    // Create a ROS subscriber for the input point cloud
    ros::Subscriber sub = nh.subscribe ("camera/depth/color/points", 10000, cloud_cb);

    // Create a ROS publisher for the output point cloud
    pub = nh.advertise<sensor_msgs::PointCloud2> ("scan_cloud", 10000);

    

    // Spin
    ros::spin ();
}
