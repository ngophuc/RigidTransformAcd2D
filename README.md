# RigidTransformAcd2D

To install the program see <a href="https://github.com/ngophuc/QuasiRegularRigidMotion/blob/master/INSTALL.txt">INSTALL.txt</a> file


If you want you can also directly test the programm online:

http://ipol-geometry.loria.fr/~phuc/ipol_demo/DecompConvexRigidMotion/


* [![Build Status](https://travis-ci.org/ngophuc/RigidTransformAcd2D.svg?branch=master)](https://travis-ci.org/ngophuc/QuasiRegularRigidMotion)

# Examples

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">flower.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./transformDecomShape -i ../Samples/flower.pgm -o ../Results/flower -e -a 0.2 -b 0.1 -t 0.8</code>
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

<p>File <a href="https://github.com/ngophuc/RigidTransformAcd2D/blob/master/Samples/flower.pgm">hand.pgm</a>: </p>&#x000A;&#x000A;
<pre class="code highlight js-syntax-highlight plaintext">
<code>./transformDecomShape -i ../Dataset/hand.pgm -o ../Dataset/hand_res.pgm -e -a 0.1 -b 0.1 -t 0.1</code>
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


