/*
 * XADArchive.h
 *
 * Copyright (c) 2017-present, MacPaw Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */
#import <Foundation/Foundation.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#import "XADException.h"
#import "CSHandle.h"
#pragma clang diagnostic pop

typedef int XADAction;
//typedef off_t xadSize; // deprecated

#define XADAbortAction 0
#define XADRetryAction 1
#define XADSkipAction 2
#define XADOverwriteAction 3
#define XADRenameAction 4

extern NSString * _Nonnull XADResourceDataKey;
extern NSString * _Nonnull XADResourceForkData;
extern NSString * _Nonnull XADFinderFlags;

NS_ASSUME_NONNULL_BEGIN

@class UniversalDetector;
@class XADArchiveParser, XADUnarchiver;

@interface XADArchive:NSObject
{
	XADArchiveParser *parser;
	XADUnarchiver *unarchiver;

	id delegate;
	NSTimeInterval update_interval;
	XADError lasterror;

	NSMutableArray *dataentries,*resourceentries;
	NSMutableDictionary *namedict;

	off_t extractsize,totalsize;
	int extractingentry;
	BOOL extractingresource;
	NSString *immediatedestination;
	BOOL immediatesubarchives,immediatefailed;
	off_t immediatesize;
	XADArchive *parentarchive;
}

+(nullable XADArchive *)archiveForFile:(NSString *)filename;
+(nullable XADArchive *)recursiveArchiveForFile:(NSString *)filename;



-(id)init;
-(nullable id)initWithFile:(NSString *)file;
-(nullable id)initWithFile:(NSString *)file error:(nullable XADError *)error;
-(nullable id)initWithFile:(NSString *)file delegate:(nullable id)del error:(nullable XADError *)error;
-(nullable id)initWithData:(NSData *)data;
-(nullable id)initWithData:(NSData *)data error:(nullable XADError *)error;
-(nullable id)initWithData:(NSData *)data delegate:(nullable id)del error:(nullable XADError *)error;
-(nullable id)initWithArchive:(XADArchive *)archive entry:(int)n;
-(nullable id)initWithArchive:(XADArchive *)archive entry:(int)n error:(nullable XADError *)error;
-(nullable id)initWithArchive:(XADArchive *)otherarchive entry:(int)n delegate:(nullable id)del error:(nullable XADError *)error;
-(nullable id)initWithArchive:(XADArchive *)otherarchive entry:(int)n
     immediateExtractionTo:(NSString *)destination error:(nullable XADError *)error;
-(nullable id)initWithArchive:(XADArchive *)otherarchive entry:(int)n
     immediateExtractionTo:(NSString *)destination subArchives:(BOOL)sub error:(nullable XADError *)error;
-(void)dealloc;

-(BOOL)_parseWithErrorPointer:(nullable XADError *)error;

-(nullable NSString *)filename;
-(nullable NSArray *)allFilenames;
-(nullable NSString *)formatName;
-(BOOL)isEncrypted;
-(BOOL)isSolid;
-(BOOL)isCorrupted;
-(int)numberOfEntries;
-(BOOL)immediateExtractionFailed;
-(nullable NSString *)commonTopDirectory;
-(nullable NSString *)comment;

-(void)setDelegate:(nullable id)delegate;
-(nullable id)delegate;

-(nullable NSString *)password;
-(void)setPassword:(nullable NSString *)newpassword;

-(NSStringEncoding)nameEncoding;
-(void)setNameEncoding:(NSStringEncoding)encoding;

-(XADError)lastError;
-(void)clearLastError;
-(nullable NSString *)describeLastError;
-(nullable NSString *)describeError:(XADError)error;

-(NSString *)description;



-(nullable NSDictionary *)dataForkParserDictionaryForEntry:(int)n;
-(nullable NSDictionary *)resourceForkParserDictionaryForEntry:(int)n;
-(nullable NSDictionary *)combinedParserDictionaryForEntry:(int)n;

-(nullable NSString *)nameOfEntry:(int)n;
-(BOOL)entryHasSize:(int)n;
-(off_t)uncompressedSizeOfEntry:(int)n;
-(off_t)compressedSizeOfEntry:(int)n;
-(off_t)representativeSizeOfEntry:(int)n;
-(BOOL)entryIsDirectory:(int)n;
-(BOOL)entryIsLink:(int)n;
-(BOOL)entryIsEncrypted:(int)n;
-(BOOL)entryIsArchive:(int)n;
-(BOOL)entryHasResourceFork:(int)n;
-(nullable NSString *)commentForEntry:(int)n;
-(nullable NSDictionary *)attributesOfEntry:(int)n;
-(nullable NSDictionary *)attributesOfEntry:(int)n withResourceFork:(BOOL)resfork;
-(nullable CSHandle *)handleForEntry:(int)n;
-(nullable CSHandle *)handleForEntry:(int)n error:(nullable XADError *)error;
-(nullable CSHandle *)resourceHandleForEntry:(int)n;
-(nullable CSHandle *)resourceHandleForEntry:(int)n error:(nullable XADError *)error;
-(nullable NSData *)contentsOfEntry:(int)n;
//-(NSData *)resourceContentsOfEntry:(int)n;

-(BOOL)extractTo:(NSString *)destination;
-(BOOL)extractTo:(NSString *)destination subArchives:(BOOL)sub;
-(BOOL)extractEntries:(NSIndexSet *)entryset to:(NSString *)destination;
-(BOOL)extractEntries:(NSIndexSet *)entryset to:(NSString *)destination subArchives:(BOOL)sub;
-(BOOL)extractEntry:(int)n to:(NSString *)destination;
-(BOOL)extractEntry:(int)n to:(NSString *)destination deferDirectories:(BOOL)defer;
-(BOOL)extractEntry:(int)n to:(NSString *)destination deferDirectories:(BOOL)defer
       resourceFork:(BOOL)resfork;
-(BOOL)extractEntry:(int)n to:(NSString *)destination deferDirectories:(BOOL)defer
           dataFork:(BOOL)datafork resourceFork:(BOOL)resfork;
-(BOOL)extractArchiveEntry:(int)n to:(NSString *)destination;

-(BOOL)_extractEntry:(int)n as:(NSString *)destfile deferDirectories:(BOOL)defer
            dataFork:(BOOL)datafork resourceFork:(BOOL)resfork;

-(void)updateAttributesForDeferredDirectories;

// Deprecated

+(NSArray *)volumesForFile:(NSString *)filename;

-(int)sizeOfEntry:(int)n;
-(void *)xadFileInfoForEntry:(int)n;
-(BOOL)extractEntry:(int)n to:(NSString *)destination overrideWritePermissions:(BOOL)override;
-(BOOL)extractEntry:(int)n to:(NSString *)destination overrideWritePermissions:(BOOL)override resourceFork:(BOOL)resfork;
-(void)fixWritePermissions;

@end



@interface NSObject (XADArchiveDelegate)

-(NSStringEncoding)archive:(XADArchive *)archive encodingForData:(NSData *)data guess:(NSStringEncoding)guess confidence:(float)confidence;
-(XADAction)archive:(XADArchive *)archive nameDecodingDidFailForEntry:(int)n data:(NSData *)data;

-(BOOL)archiveExtractionShouldStop:(XADArchive *)archive;
-(BOOL)archive:(XADArchive *)archive shouldCreateDirectory:(NSString *)directory;
-(XADAction)archive:(XADArchive *)archive entry:(int)n collidesWithFile:(NSString *)file newFilename:(NSString * _Nonnull * _Nonnull)newname;
-(XADAction)archive:(XADArchive *)archive entry:(int)n collidesWithDirectory:(NSString *)file newFilename:(NSString * _Nonnull * _Nonnull)newname;
-(XADAction)archive:(XADArchive *)archive creatingDirectoryDidFailForEntry:(int)n;

-(void)archiveNeedsPassword:(XADArchive *)archive;

-(void)archive:(XADArchive *)archive extractionOfEntryWillStart:(int)n;
-(void)archive:(XADArchive *)archive extractionProgressForEntry:(int)n bytes:(off_t)bytes of:(off_t)total;
-(void)archive:(XADArchive *)archive extractionOfEntryDidSucceed:(int)n;
-(XADAction)archive:(XADArchive *)archive extractionOfEntryDidFail:(int)n error:(XADError)error;
-(XADAction)archive:(XADArchive *)archive extractionOfResourceForkForEntryDidFail:(int)n error:(XADError)error;

-(void)archive:(XADArchive *)archive extractionProgressBytes:(off_t)bytes of:(off_t)total;
-(void)archive:(XADArchive *)archive extractionProgressFiles:(int)files of:(int)total;

// Deprecated
-(NSStringEncoding)archive:(XADArchive *)archive encodingForName:(const char *)bytes guess:(NSStringEncoding)guess confidence:(float)confidence;
-(XADAction)archive:(XADArchive *)archive nameDecodingDidFailForEntry:(int)n bytes:(const char *)bytes;

@end


#ifndef XAD_NO_DEPRECATED

#define XADAbort XADAbortAction
#define XADRetry XADRetryAction
#define XADSkip XADSkipAction
#define XADOverwrite XADOverwriteAction
#define XADRename XADRenameAction

typedef XADError xadERROR;
typedef off_t xadSize;

#define XADERR_NO XADNoError
#if 0
#define XADUnknownError          0x0001 /* unknown error */
#define XADInputError            0x0002 /* input data buffers border exceeded */
#define XADOutputError           0x0003 /* output data buffers border exceeded */
#define XADBadParametersError    0x0004 /* function called with illegal parameters */
#define XADOutOfMemoryError      0x0005 /* not enough memory available */
#define XADIllegalDataError      0x0006 /* data is corrupted */
#define XADNotSupportedError     0x0007 /* command is not supported */
#define XADResourceError         0x0008 /* required resource missing */
#define XADDecrunchError         0x0009 /* error on decrunching */
#define XADFiletypeError         0x000A /* unknown file type */
#define XADOpenFileError         0x000B /* opening file failed */
#define XADSkipError             0x000C /* file, disk has been skipped */
#define XADBreakError            0x000D /* user break in progress hook */
#define XADFileExistsError       0x000E /* file already exists */
#define XADPasswordError         0x000F /* missing or wrong password */
#define XADMakeDirectoryError    0x0010 /* could not create directory */
#define XADChecksumError         0x0011 /* wrong checksum */
#define XADVerifyError           0x0012 /* verify failed (disk hook) */
#define XADGeometryError         0x0013 /* wrong drive geometry */
#define XADDataFormatError       0x0014 /* unknown data format */
#define XADEmptyError            0x0015 /* source contains no files */
#define XADFileSystemError       0x0016 /* unknown filesystem */
#define XADFileDirectoryError    0x0017 /* name of file exists as directory */
#define XADShortBufferError      0x0018 /* buffer was too short */
#define XADEncodingError         0x0019 /* text encoding was defective */
#endif

#define XADAbort XADAbortAction
#define XADRetry XADRetryAction
#define XADSkip XADSkipAction
#define XADOverwrite XADOverwriteAction
#define XADRename XADRenameAction

#endif

NS_ASSUME_NONNULL_END
