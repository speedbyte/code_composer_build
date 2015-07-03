<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" omit-xml-declaration="no"/>
	<xsl:template match="/sourcemonitor_metrics">
		<html>
			<head>
		   	<meta name="Author" content="S1nn"/>
		   	<title><xsl:value-of select="project/project_name"/></title>
			</head>
			<body>
				<table width="100%">
					<xsl:attribute name="summary">
						<xsl:value-of select="project/project_name"/>
					</xsl:attribute>
					<tr>
						<td>
							<span id="data">
  								<xsl:apply-templates select="project"/>
							</span>
						</td>
					</tr>
				</table>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="project">
		<table width="100%">
			<xsl:attribute name="summary">
				<xsl:value-of select="project_name"/>
			</xsl:attribute>
			<tr bgcolor="#C0C0C0" tabIndex="0">
				<td><b>file_name</b></td>
				<xsl:for-each select="metric_names/metric_name">
					<td align="center"><b><xsl:value-of select="."/></b></td>
				</xsl:for-each>
			</tr>
			<xsl:for-each select="checkpoints/checkpoint/files/file">
				<xsl:sort select="@file_name"/>
				<xsl:choose>
					<xsl:when test="position() mod 2 != 0">
						<tr>
							<td><b><xsl:value-of select="@file_name"/></b></td>
							<xsl:for-each select="metrics/metric">
								<td align="center"><b><xsl:value-of select="."/></b></td>
							</xsl:for-each>
						</tr>
					</xsl:when>
					<xsl:otherwise>
						<tr bgcolor="#C0C0C0">
							<td><b><xsl:value-of select="@file_name"/></b></td>
							<xsl:for-each select="metrics/metric">
								<td align="center"><b><xsl:value-of select="."/></b></td>
							</xsl:for-each>
						</tr>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
		</table>
	</xsl:template>
</xsl:stylesheet>
