#pragma once

//#include "Legolas/Block/Expression.hxx"
#include "Legolas/Array/Block/BaseBlock.hxx"
#include "type_traits"

namespace Legolas{


    struct AddBlock{
      template <class LEFT, class RIGHT>
      static inline
      const auto apply(const LEFT & left, const RIGHT & right){
        //auto apply(LEFT left, RIGHT right){
        return left+right;
      }
    };

    struct MinusBlock{
      template <class LEFT, class RIGHT>
      static inline
      auto apply(const LEFT & left, const RIGHT & right) -> decltype(left-right){
        //auto apply(LEFT left, RIGHT right){
        return left-right;
      }
    };

    struct MultBlock{
      template <class LEFT, class RIGHT>
      static inline
      auto apply(const LEFT & left, const RIGHT & right)  -> decltype(left*right){
        //auto apply(LEFT left, RIGHT right){
        return left*right;
      }
    };



    template <class LEFT, class BINARY_OPERATOR , class RIGHT>
    struct BlockExpression: public BaseBlock< BlockExpression<LEFT,BINARY_OPERATOR,RIGHT> >{

        using Left=LEFT;
        using Right=RIGHT;
        using ScalarType=typename Left::ScalarType;
        using Shape=typename Left::Shape;

        const Left  left_;
        const Right right_;


        BlockExpression(const BaseBlock<LEFT> & left, const BaseBlock<RIGHT> & right):left_(left.getBlockRef()),
            right_(right.getBlockRef())
        {
            static_assert(std::is_same<typename LEFT::Shape, typename RIGHT::Shape>::value,"Block Shapes differ");
            static_assert(std::is_same<typename LEFT::ScalarType, typename RIGHT::ScalarType>::value,"Block Types differ");
        }


        inline auto getEigenView( void ) const {
            return BINARY_OPERATOR::apply(left_.getEigenView(),right_.getEigenView());
        }



    };

    template <class BLOCK>
    struct ScaledBlock: public BaseBlock< ScaledBlock<BLOCK> >{

        using Block=BLOCK;
        using ScalarType=typename Block::ScalarType;
        using Shape=typename Block::Shape;

        const Block block_;
        const ScalarType coef_;

        ScaledBlock(const BaseBlock<Block> & block, ScalarType coef):
            block_(block.getBlockRef()),
            coef_(coef){}

        inline auto getEigenView( void ) const{
            return block_.getEigenView()*coef_;
        }


    };


    template <class LEFT, class RIGHT>
    BlockExpression<LEFT,AddBlock,RIGHT> operator+ (const BaseBlock<LEFT> & left, const BaseBlock<RIGHT> & right){
        return BlockExpression<LEFT,AddBlock,RIGHT>(left,right);
    }
    template <class LEFT, class RIGHT>
    BlockExpression<LEFT,MinusBlock,RIGHT> operator- (const BaseBlock<LEFT> & left, const BaseBlock<RIGHT> & right){
        return BlockExpression<LEFT,MinusBlock,RIGHT>(left,right);
    }

    template <class LEFT, class RIGHT>
    BlockExpression<LEFT,MultBlock,RIGHT> operator* (const BaseBlock<LEFT> & left, const BaseBlock<RIGHT> & right){
        return BlockExpression<LEFT,MultBlock,RIGHT>(left,right);
    }

    template <class Block>
    ScaledBlock<Block> operator*(double coef, const BaseBlock<Block> & right){
        return ScaledBlock<Block>(right,typename Block::ScalarType(coef));
    }

    template <class Block>
    ScaledBlock<Block> operator*(const BaseBlock<Block> & right, double coef){
        return ScaledBlock<Block>(right,typename Block::ScalarType(coef));
    }





}
