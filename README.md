# RigidTransformAcd2D

To install the program see <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/INSTALL.txt">INSTALL.txt</a> file


If you want you can also directly test the programm online:

http://ipol-geometry.loria.fr/~phuc/ipol_demo/DecompConvexRigidMotion/


* [![Build Status](https://travis-ci.org/ngophuc/RigidTransformAcd2D.svg?branch=master)](https://travis-ci.org/ngophuc/RigidTransformAcd2D)

# Program parameters

Allowed options are: 
<br>
<table align="right">
  <tr>
    <th align=Left>Options</th>
    <th align=Left>Description</th>
  </tr>
  <tr>
    <td align=Left>-h [ --help ]</th>
    <td>display this message.</th>
  </tr>	
  <tr>
    <td align=Left>-i [ --input ] arg</td>
    <td align=Left>input filename.</td>
  </tr>
  <tr>
    <td align=Left>-o [ --output ] arg (=./) </td>
    <td align=Left>output dir (default ./).</td>
  </tr>
  <tr>
    <td align=Left>-d [ --dir ] arg (=./) </td>
    <td align=Left> execute dir (default ./).</td>
  </tr>
  <tr>
    <td align=Left>-x [ --tx ] arg (=0) </td>
    <td align=Left>x-translation.</td>
  </tr>	
  <tr>
    <td align=Left>-y [ --ty ] arg (=0)</th>
    <td>y-translation.</th>
  </tr>	
  <tr>
    <td align=Left>-t [ --theta ] arg (=0)  </td>
    <td align=Left>rotation angle.</td>
  </tr>
  <tr>
    <td align=Left>-r [ --regular ]  </td>
    <td align=Left>With quasi-regular verification.</td>
  </tr>
  <tr>
    <td align=Left>-e [ --eps ] </td>
    <td align=Left>Save output with eps format.</td>
  </tr>
</table>
<br><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/>

# Examples

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">flower.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./transformDecomShape2d -i ../Samples/flower.pgm -o ../Results/flower_result.pgm -e -r -a 0.2 -b 0.1 -t 0.8</code>
</pre>&#x000A;&#x000A;
<p>
	<table cellpadding="5">
		<tr>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.png">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.png" alt="Input image" />
			</a>	
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_poly.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_poly.png" alt="Polygonalization" />
			</a>
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_decomp.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_decomp.png" alt="Convex decomposition" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_shape.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_shape.png" alt="Digitized decomposition" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_result.pmg">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/flower_result.png" alt="Transformed result" />
			</a>
		</td>  
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Polygon extraction </td>
			<td align="center" valign="center">  Convex decomposition </td>
			<td align="center" valign="center">  Digitized decomposition </td>
			<td align="center" valign="center">  Transformed result </td>
		</tr>
	</table>
</p>

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">leaf.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./transformDecomShape2d -i ../Samples/leaf.pgm -o ../Results/leaf_result.pgm -e -r -a 0.1 -b 0.1 -t 0.314</code>
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
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_poly.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_poly.png" alt="Polygonalization" />
			</a>
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_decomp.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_decomp.png" alt="Convex decomposition" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_shape.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_shape.png" alt="Digitized decomposition" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_result.pmg">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/leaf_result.png" alt="Transformed result" />
			</a>
		</td>  
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Polygon extraction </td>
			<td align="center" valign="center">  Convex decomposition </td>
			<td align="center" valign="center">  Digitized decomposition </td>
			<td align="center" valign="center">  Transformed result </td>
		</tr>
	</table>
</p>
<!--
<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">snowflake.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./transformDecomShape2d -i ../Samples/snowflake.pgm -o ../Results/snowflake_result.pgm -e -r -a 0.12 -b 0.41 -t 0.14</code>
</pre>&#x000A;&#x000A;
<p>
	<table cellpadding="5">
		<tr>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/snowflake.png">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/snowflake.png" alt="Input image" />
			</a>	
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_poly.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_poly.png" alt="Polygonalization" />
			</a>
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_decomp.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_decomp.png" alt="Convex decomposition" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_shape.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_shape.png" alt="Digitized decomposition" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_result.pmg">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/snowflake_result.png" alt="Transformed result" />
			</a>
		</td>  
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Polygon extraction </td>
			<td align="center" valign="center">  Convex decomposition </td>
			<td align="center" valign="center">  Digitized decomposition </td>
			<td align="center" valign="center">  Transformed result </td>
		</tr>
	</table>
</p>
-->
<!--
<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">hand.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./transformDecomShape2d -i ../Samples/hand.pgm -o ../Results/hand_result.pgm -e -r -a 0.1 -b 0.1 -t 0.1</code>
</pre>&#x000A;&#x000A;
<p>
	<table cellpadding="5">
		<tr>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/hand.png">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/hand.png" alt="Input image" />
			</a>	
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_poly.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_poly.png" alt="Polygonalization" />
			</a>
		</td>	
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_decomp.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_decomp.png" alt="Convex decomposition" />
			</a>
		</td>
		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_shape.eps">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_shape.png" alt="Digitized decomposition" />
			</a>
		</td>
    		<td align="center" valign="center">
			<a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_result.pmg">
				<img width="120" src="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Results/hand_result.png" alt="Transformed result" />
			</a>
		</td>  
		</tr>
		<tr>
			<td align="center" valign="center">  Input image </td>
			<td align="center" valign="center">  Polygon extraction </td>
			<td align="center" valign="center">  Convex decomposition </td>
			<td align="center" valign="center">  Digitized decomposition </td>
			<td align="center" valign="center">  Transformed result </td>
		</tr>
	</table>
</p>
-->
