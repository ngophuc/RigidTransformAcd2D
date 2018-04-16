# RigidTransformAcd2D

To install the program see <a href="https://github.com/ngophuc/QuasiRegularRigidMotion/blob/master/INSTALL.txt">INSTALL.txt</a> file


If you want you can also directly test the programm online:

http://ipol-geometry.loria.fr/~phuc/ipol_demo/DecompConvexRigidMotion/


* [![Build Status](https://travis-ci.org/ngophuc/RigidTransformAcd2D.svg?branch=master)](https://travis-ci.org/ngophuc/QuasiRegularRigidMotion)

# Examples

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">flower.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./testRigidTransform -i ../Samples/flower.pgm -o ../Results/flower -e -a 0.1 -b 0.3 -t 0.314</code>
</pre>&#x000A;&#x000A;
<p>
	<table cellpadding="5">
		<tr>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.png">
				<img width="130" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.png" alt="Input image" />
			</a>	
		</td>		
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_points.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_points.png" alt="Input points" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_tpoint.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_tpoint.png" alt="T_point" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_thull.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_thull.png" alt="T_hull" />
			</a>
		</td>  
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_tpoly.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_tpoly.png" alt="T_poly" />
			</a>
		</td>  	
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Input points </td>
			<td align="center" valign="center">  Rigid motion point-by-point </td>
			<td align="center" valign="center">  Rigid motion by convex hull </td>
			<td align="center" valign="center">  Rigid motion by polygonalization </td>
		</tr>
	</table>
</p>

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/leaf.pgm">leaf.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./testRigidTransform -i ../Samples/leaf.pgm -o ../Results/leaf -e -a 0.1 -b 0.1 -t 0.314</code>
</pre>&#x000A;&#x000A;
<p>
	<table cellpadding="5">
		<tr>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/leaf.png">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/leaf.png" alt="Input image" />
			</a>
		</td>		
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_points.eps">
				<img width="110" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_points.png" alt="Input points" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_tpoint.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_tpoint.png" alt="T_point" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_thull.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_thull.png" alt="T_hull" />
			</a>
		</td>  
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_tpoly.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_tpoly.png" alt="T_poly" />
			</a>
		</td>  	
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Input points </td>
			<td align="center" valign="center">  Rigid motion point-by-point </td>
			<td align="center" valign="center">  Rigid motion by convex hull </td>
			<td align="center" valign="center">  Rigid motion by polygonalization </td>
		</tr>
	</table>
</p>

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/snowflake.pgm">snowflake.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./testRigidTransform -i ../Samples/snowflake.pgm -o ../Results/snowflake -e -a 0.2 -b 0.2 -t 0.314</code>
</pre>&#x000A;&#x000A;
<p>
	<table cellpadding="5">
		<tr>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/snowflake.png">
				<img width="125" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/snowflake.png" alt="Input image" />
			</a>
		</td>		
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_points.eps">
				<img width="110" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_points.png" alt="Input points" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_tpoint.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_tpoint.png" alt="T_point" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_thull.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_thull.png" alt="T_hull" />
			</a>
		</td>  
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_tpoly.eps">
				<img width="100" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_tpoly.png" alt="T_poly" />
			</a>
		</td>  	
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Input points </td>
			<td align="center" valign="center">  Rigid motion point-by-point </td>
			<td align="center" valign="center">  Rigid motion by convex hull </td>
			<td align="center" valign="center">  Rigid motion by polygonalization </td>
		</tr>
	</table>
</p>
